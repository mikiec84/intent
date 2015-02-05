#include <atomic>
#include <map>
#include <mutex>
#include <thread>

#include <asio.hpp>

#include "core/net/curl/.private/libcurl_callbacks.h"
#include "core/net/curl/.private/channel-impl.h"
#include "core/net/curl/.private/curl_multi.h"
#include "core/net/curl/channel.h"
#include "core/net/curl/session.h"


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

// Curl needs to be initialized once, globally, and cleaned up after
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
	static atomic<uint32_t> the_next_id;
	return the_next_id.fetch_add(1);
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
	service_runner.join();
	still_running = 0;
	while (!sessions.empty()) {
		auto x = sessions.begin();
		auto session = x->second;
		if (session) {
			delete session;
		}
		sessions.erase(x);
	}
	curl_multi_cleanup(multi);
}


channel::channel(): impl(new impl_t()) {
}


channel::~channel() {
	delete impl;
}


channel & channel::get_default() {
	static channel the_channel;
	return the_channel;
}


uint32_t channel::get_id() const {
	return impl->id;
}


void channel::open() {
	auto & svc = impl->io_service;
	if (svc.stopped()) {
		impl->service_runner = thread([&svc]() { svc.run(); });
	}
}


void channel::register_session(session * s, bool add) {
	if (s) {
		lock_guard<mutex> lock(impl->mtx);
		auto & sessions = impl->sessions;
		if (add) {
			sessions[s->get_id()] = s;
		} else {
			auto x = sessions.find(s->get_id());
			if (x != sessions.end()) {
				sessions.erase(x);
			}
		}
	}
}


/* Check for completed transfers, and remove their easy handles */
void channel::impl_t::check_multi_info() {

	char *eff_url;
	CURLMsg *msg;
	int msgs_left;
	session::impl_t * simpl;
	CURL *easy;
	CURLcode res;

	fprintf(stdout, "\nREMAINING: %d", still_running);

	while ((msg = curl_multi_info_read(multi, &msgs_left))) {
		if (msg->msg == CURLMSG_DONE) {
			easy = msg->easy_handle;
			res = msg->data.result;
			curl_easy_getinfo(easy, CURLINFO_PRIVATE, &simpl);
			curl_easy_getinfo(easy, CURLINFO_EFFECTIVE_URL, &eff_url);
			fprintf(stdout, "\nDONE: %s => (%d) %s", eff_url, res, "simpl->error");
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

	CURLMcode rc;
	rc = curl_multi_socket_action(multi, tcp_socket->native_handle(), action,
								&still_running);

	mcode_or_die("on_socket_event: curl_multi_socket_action", rc);
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

		CURLMcode rc;
		rc = curl_multi_socket_action(multi, CURL_SOCKET_TIMEOUT, 0, &still_running);

		mcode_or_die("on_timeout: curl_multi_socket_action", rc);
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
