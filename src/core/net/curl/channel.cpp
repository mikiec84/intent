#include <atomic>
#include <map>
#include <mutex>
#include <thread>

#include "core/net/curl/.private/libcurl_callbacks.h"
#include "core/net/curl/.private/channel-impl.h"
#include "core/net/curl/.private/session-impl.h"
#include "core/net/curl/.private/multi.h"
#include "core/net/curl/channel.h"
#include "core/net/curl/session.h"
#include "core/util/monotonic_id.h"


using asio::deadline_timer;
using asio::io_service;
using asio::error_code;

using std::atomic;
using std::lock_guard;
using std::map;
using std::mutex;
using std::string;
using std::thread;

using namespace std::placeholders;


namespace intent {
namespace core {
namespace net {
namespace curl {


typedef asio::ip::tcp::socket asio_socket_t;
typedef map<curl_socket_t, asio_socket_t *> socket_map_t;
typedef std::pair<curl_socket_t, asio_socket_t *> socket_pair_t;

// Curl needs to be initialized once per process, globally, and cleaned up after
// main() exits. Define an object that does init and cleanup in its ctor and dtor;
// we will create a static (global) instance of it to guarantee this lifecycle.
struct env {
	env(long flags) { curl_global_init(flags); }
	~env() { curl_global_cleanup(); }
};


// Create a static instance of the init/cleanup object the first time
// this function is called.
env const & get_env(long flags) {
	static env the_env(flags);
	return the_env;
}


static uint32_t get_next_id() {
	static util::monotonic_id<uint32_t> the_generator;
	return the_generator.next();
}


channel::impl_t::impl_t() : id(get_next_id()), multi(), still_running(1),
		io_service(), timeout(io_service), socket_map(), service_runner(),
		// Add some abstract "work" so that a call to io_service.run() will
		// never exit because it always sees something pending...
		work(io_service), sessions(), mtx() {

	curl_multi_setopt(multi, CURLMOPT_SOCKETFUNCTION, libcurl_callbacks::on_socket_update);
	curl_multi_setopt(multi, CURLMOPT_SOCKETDATA, this);

	curl_multi_setopt(multi, CURLMOPT_TIMERFUNCTION, libcurl_callbacks::on_change_timeout);
	curl_multi_setopt(multi, CURLMOPT_TIMERDATA, this);
}


channel::impl_t::~impl_t() {
	lock_guard<mutex> lock(mtx);
	if (!io_service.stopped()) {
		io_service.stop();
	}
	if (is_open()) {
		still_running = 0;
		while (!sessions.empty()) {
			auto x = sessions.begin();
			auto session = x->second;
			if (session) {
				// Avoid deadlock by breaking backref.
				session->impl->channel.reset();
			}
			// Here's where we do auto-deregistration. This also calls the
			// dtor of the shared_ptr, which in turn calls the dtor of the
			// session, which (since it's been detached from us) *won't*
			// turn around and tell us to remove it from our list.
			sessions.erase(x);
		}
	}
}


channel::channel(): impl(new impl_t()) {
}


channel::~channel() {
	printf("channel %u dtor\n", get_id());
	delete impl;
}


channel_handle channel::get_default() {
	static channel_handle the_handle(new channel());
	return the_handle;
}


uint32_t channel::get_id() const {
	return impl->id;
}


static const std::thread::id inert_thread_id;


void channel::open() {

	lock_guard<mutex> lock(impl->mtx);

	if (!impl->is_open()) {
		fprintf(stderr, "Opening channel %u.\n", get_id());
		auto & svc = impl->io_service;
		impl->service_runner = thread([&svc]() {
			fprintf(stderr, "Entering asio service runner thread.\n");
			asio::io_service::work never_done(svc);
			svc.run();
			fprintf(stderr, "Leaving asio service runner thread.\n");
		});
		impl->service_runner.detach();
	}
}


bool channel::impl_t::is_open() const {
	return service_runner.get_id() != inert_thread_id;
}


bool channel::is_open() const {
	lock_guard<mutex> lock(impl->mtx);

	return impl->is_open();
}


void channel::attach(session * s) {
	if (s) {
		lock_guard<mutex> lock(impl->mtx);
		impl->sessions[s->get_id()] = s;
	}
}


void channel::detach(unsigned session_id) {
	lock_guard<mutex> lock(impl->mtx);
	auto & sessions = impl->sessions;
	auto x = sessions.find(session_id);
	if (x != sessions.end()) {
		// This calls the dtor of the smart ptr, which (since ref count
		// reaches 0) will also call the dtor of the session.
		sessions.erase(x);
	}
}


/* Check for completed transfers, and remove their easy handles */
void channel::impl_t::check_multi_info() {

	char *eff_url;
	CURLMsg *msg;
	int msgs_left;
	session::impl_t * simpl;

	fprintf(stdout, "\nREMAINING: %d", still_running);

	while ((msg = curl_multi_info_read(multi, &msgs_left))) {
		if (msg->msg == CURLMSG_DONE) {
			auto easy = msg->easy_handle;
			auto res = msg->data.result;
			curl_easy_getinfo(easy, CURLINFO_PRIVATE, &simpl);
			curl_easy_getinfo(easy, CURLINFO_EFFECTIVE_URL, &eff_url);
			fprintf(stdout, "\nDONE: %s => (%d) %s", eff_url, res, "simpl->error");
			//curl_easy_getinfo(easy, CURLINFO_RESPONSE_CODE, &http_code);
			curl_multi_remove_handle(multi, easy);
			//free(simpl->url);
			//simpl->url = nullptr;
			//curl_easy_cleanup(easy);
			//impl->close();
		}
	}
}


/* Called by asio when there is an action on a socket */
void channel::impl_t::on_socket_event(asio_socket_t *tcp_socket,
					 int action)
{
	fprintf(stdout, "\nevent_cb: action=%d", action);

	auto rc = curl_multi_socket_action(multi, tcp_socket->native_handle(),
			action, &still_running);

	mcode_or_die("on_socket_event: multi_socket_action", rc);
	check_multi_info();

	if (still_running <= 0) {
		fprintf(stdout, "\nlast transfer done, kill timeout");
		timeout.cancel();
	}
}


/* Called by asio when our timeout expires */
void channel::impl_t::on_timeout(error_code const & error)
{
	if (!error) {
		fprintf(stdout, "\ntimer_cb: ");

		auto rc = curl_multi_socket_action(multi, CURL_SOCKET_TIMEOUT, 0, &still_running);

		mcode_or_die("on_timeout: multi_socket_action", rc);
		check_multi_info();
	}
}


/* Clean up any data */
void channel::impl_t::remove_socket(int *f) {
	fprintf(stdout, "\nremsock: ");

	if (f) {
		free(f);
	}
}


void channel::impl_t::change_socket_action(int *fdp, curl_socket_t sock, CURL * easy, int act)
{
	fprintf(stdout, "\nsetsock: socket=%d, act=%d, fdp=%p", sock, act, fdp);

	auto it = socket_map.find(sock);

	if (it == socket_map.end()) {
		fprintf(stdout, "\nsocket %d is a c-ares socket, ignoring", sock);
		return;
	}

	asio_socket_t * tcp_socket = it->second;

	*fdp = act;

	if (act == CURL_POLL_IN)
	{
		fprintf(stdout, "\nwatching for socket to become readable");

		tcp_socket->async_read_some(asio::null_buffers(),
									std::bind(&impl_t::on_socket_event, this, tcp_socket, act));
	} else if (act == CURL_POLL_OUT) {
		fprintf(stdout, "\nwatching for socket to become writable");

		tcp_socket->async_write_some(asio::null_buffers(),
									 std::bind(&impl_t::on_socket_event, this, tcp_socket, act));
	} else if (act == CURL_POLL_INOUT) {
		fprintf(stdout, "\nwatching for socket to become readable & writable");

		tcp_socket->async_read_some(asio::null_buffers(),
									std::bind(&impl_t::on_socket_event, this, tcp_socket, act));

		tcp_socket->async_write_some(asio::null_buffers(),
									 std::bind(&impl_t::on_socket_event, this, tcp_socket, act));
	}
}


void channel::impl_t::add_socket(curl_socket_t sock, CURL * easy, int action)
{
	/* fdp is used to store current action */
	int *fdp = (int *) calloc(sizeof(int), 1);

	change_socket_action(fdp, sock, easy, action);
	curl_multi_assign(multi, sock, fdp);
}


}}}} // end namespace
