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

char const * get_socket_state_descrip(int state);


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

    curl_multi_setopt(multi, CURLMOPT_TIMERFUNCTION, libcurl_callbacks::on_adjust_timeout);
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
                session->impl->channel = nullptr;
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
    fprintf(stderr, "channel %u dtor\n", get_id());
    delete impl;
}


channel & channel::get_default() {
    static channel the_handle;
    return the_handle;
}


uint32_t channel::get_id() const {
    return impl->id;
}


static const std::thread::id inert_thread_id;


void channel::open() {

    lock_guard<mutex> lock(impl->mtx);

    if (!impl->is_open()) {
        auto & svc = impl->io_service;

        impl->service_runner = thread([&svc]() {
            asio::io_service::work never_done(svc);
            svc.run();
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
void channel::impl_t::handle_done_transfers() {

    CURLMsg *msg;
    int msgs_left;
    session::impl_t * simpl;

    while ((msg = curl_multi_info_read(multi, &msgs_left)) != nullptr) {

        // It is common to get a CURLMSG_DONE when we are done--but it is not
        // guaranteed in cases where the remote server closes a socket without
        // warning. Testing also shows that we don't necessarily receive this
        // message at the end of chunked transfer encoding with persistent
        // connections. Therefore, this block is not the only place where we call
        // cleanup_after_transfer().
        if (msg->msg == CURLMSG_DONE) {
            auto easy = msg->easy_handle;
            curl_easy_getinfo(easy, CURLINFO_PRIVATE, &simpl);
            simpl->cleanup_after_transfer();
        }
    }
}


static inline bool socket_state_implies_pending_transfer(int state) {
    switch (state) {
    case CURL_POLL_IN:
    case CURL_POLL_OUT:
    case CURL_POLL_INOUT:
        return true;
    default:
        return false;
    }
}


/**
 * Called by asio when there is an event that changes status of a socket. Since
 * recent asio versions are edge-triggered, we should assume we only get one
 * shot at noticing a state transition.
 */
void channel::impl_t::on_socket_event(asio_socket_t *tcp_socket, int event, int * socket_state)
{
    int start_state = *socket_state;

    auto rc = curl_multi_socket_action(multi, tcp_socket->native_handle(),
            event, &still_running);

    mcode_or_die("on_socket_event: multi_socket_action", rc);

    // If the read/write that just finished wasn't enough to satisfy curl
    // (a state we recognize by seeing if curl left the desired socket state
    // unchanged when we called curl_multi_socket_action), then schedule another
    // read/write to happen as soon as the socket is ready again.
    if (*socket_state == start_state && socket_state_implies_pending_transfer(start_state)) {
        queue_callback_when_socket_is_ready(socket_state, tcp_socket);
    }

    handle_done_transfers();

    if (still_running <= 0) {
        timeout.cancel();
    }
}


/* Called by asio when our timeout expires */
void channel::impl_t::on_timeout(error_code const & error)
{
    if (!error) {
        auto rc = curl_multi_socket_action(multi, CURL_SOCKET_TIMEOUT, 0, &still_running);
        mcode_or_die("on_timeout: multi_socket_action", rc);
        handle_done_transfers();
    }
}


/* Clean up any data */
void channel::impl_t::remove_socket(int * socket_state) {
    *socket_state = CURL_POLL_NONE;
}


void channel::impl_t::queue_callback_when_socket_is_ready(int * socket_state, asio_socket_t * tcp_socket) {
    int desired_state = *socket_state;
    #define handler std::bind(&impl_t::on_socket_event, this, tcp_socket, desired_state, socket_state)
    if (desired_state == CURL_POLL_IN)
    {
        tcp_socket->async_read_some(asio::null_buffers(), handler);

    } else if (desired_state == CURL_POLL_OUT) {
        tcp_socket->async_write_some(asio::null_buffers(), handler);

    } else if (desired_state == CURL_POLL_INOUT) {
        tcp_socket->async_read_some(asio::null_buffers(), handler);
        tcp_socket->async_write_some(asio::null_buffers(), handler);

    } else {
        fprintf(stderr, "desired_state = %d (unknown)\n", desired_state);
    }
    #undef handler
}


void channel::impl_t::change_socket_state(int * socket_state, curl_socket_t sock, CURL * easy, int desired_state)
{
    auto it = socket_map.find(sock);

    if (it == socket_map.end()) {
        fprintf(stderr, "socket %d is a c-ares socket, ignoring\n", sock);
        return;
    }

    asio_socket_t * tcp_socket = it->second;

    *socket_state = desired_state;

    queue_callback_when_socket_is_ready(socket_state, tcp_socket);
}


void channel::impl_t::add_socket(curl_socket_t sock, CURL * easy, int desired_state)
{
    char * x;
    curl_easy_getinfo(easy, CURLINFO_PRIVATE, &x);
    session::impl_t * simpl = reinterpret_cast<session::impl_t *>(x);

    // Tell curl to give us this pointer any time we interact with the socket.
    curl_multi_assign(multi, sock, simpl);

    // Now invoke the logic that happens when curl tells us to change our state.
    change_socket_state(&simpl->curl_socket_state, sock, easy, desired_state);
}


}}}} // end namespace
