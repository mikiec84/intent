#include <map>
#include <thread>

#include <asio.hpp>
#define CURL_STATICLIB 1
#include <curl/curl.h>

#include "core/net/curl.h"


using asio::deadline_timer;
using asio::io_service;
using asio::error_code;

using std::atomic;
using std::map;
using std::thread;

using namespace std::placeholders;

namespace intent {
namespace core {
namespace net {


typedef asio::ip::tcp::socket asio_socket_t;
typedef map<curl_socket_t, asio_socket_t *> socket_map_t;
typedef std::pair<curl_socket_t, asio_socket_t *> socket_pair_t;


static void mcode_or_die(const char *where, CURLMcode code);


struct libcurl_callbacks {
    typedef curl_channel::impl_t chimpl_t;

    static int on_change_timeout(CURLM * multi, long timeout_ms, void * _chimpl);
    static curl_socket_t on_open_socket(void * _chimpl, curlsocktype purpose, curl_sockaddr *address);
    static int on_close_socket(void * _chimpl, curl_socket_t item);
    static int on_socket_update(CURL * easy, curl_socket_t sock, int what, void * _chimpl, void * sockp);
    static int on_progress(void * _session, uint64_t expected_receive_total, uint64_t received_so_far, uint64_t expected_send_total, uint64_t sent_so_far);
    static size_t on_receive_data(void * data, size_t size_per_record, size_t num_records, void * _session);
};



// Curl needs to be initialized once, globally, and cleaned up after
// main() exits. Define an object that does init and cleanup in its ctor and dtor;
// we will create a static (global) instance of it to guarantee this lifecycle.
struct env {
    env(long flags) { curl_global_init(flags); }
    ~env() { curl_global_cleanup(); }
};


// Create a static instance of the init/cleanup object the first time
// this function is called.
env const & get_curl_env(long flags) {
    static env the_env(flags);
    return the_env;
}


// Any time we call curl_multi_init(), make sure the global init happens
// first. This gives us JIT initialization of curl, with destruction after
// main()...
CURLM * wrapped_curl_multi_init(long flags = CURL_GLOBAL_ALL) {
    get_curl_env(flags);
    return curl_multi_init();
}


struct curl_channel::impl_t {

    CURLM * multi;
    int still_running;
    io_service io_service;
    deadline_timer timeout;
    socket_map_t socket_map;
    thread service_runner;
    io_service::work work;

    impl_t() : multi(wrapped_curl_multi_init()), still_running(1),
            io_service(), timeout(io_service), socket_map(), service_runner(),
            // Add some abstract "work" so that a call to io_service.run() will
            // never exit because it always sees something pending...
            work(io_service) {

        curl_multi_setopt(multi, CURLMOPT_SOCKETFUNCTION, libcurl_callbacks::on_socket_update);
        curl_multi_setopt(multi, CURLMOPT_SOCKETDATA, this);

        curl_multi_setopt(multi, CURLMOPT_TIMERFUNCTION, libcurl_callbacks::on_change_timeout);
        curl_multi_setopt(multi, CURLMOPT_TIMERDATA, this);
    }

    ~impl_t() {
        still_running = 0;
        curl_multi_cleanup(multi);
    }

    void on_socket_event(asio_socket_t * tcp_socket, int action);
    void on_timeout(error_code const & error);

    void add_socket(curl_socket_t sock, CURL *easy, int action);
    void change_socket_action(int *fdp, curl_socket_t sock, CURL*easy, int act);
    void check_multi_info();
    void remove_socket(int *f);

};


curl_channel::curl_channel(): impl(new impl_t()) {
}


curl_channel::~curl_channel() {
    if (!impl->io_service.stopped()) {
        impl->io_service.stop();
    }
    impl->service_runner.join();
    curl_multi_cleanup(impl->multi);
    delete impl;
}


void curl_channel::open() {
    auto & svc = impl->io_service;
    if (svc.stopped()) {

        impl->service_runner = thread([&svc]() { svc.run(); });
    }
}


struct curl_session::impl_t {

    curl_channel & channel;
    CURL *easy;
    char *url;
    char error[CURL_ERROR_SIZE];
    curl_session::receive_callback receive_func;
    curl_session::progress_callback progress_func;

    impl_t(curl_channel & channel,
            curl_session::receive_callback receive_func,
            curl_session::progress_callback progress_func):
        channel(channel),
        easy(curl_easy_init()),
        url(nullptr),
        receive_func(receive_func),
        progress_func(progress_func) {

        easy = curl_easy_init();
        if (!easy) {
            fprintf(stdout, "\ncurl_easy_init() failed, exiting!");
            exit(2);
        }

        curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION, libcurl_callbacks::on_receive_data);
        curl_easy_setopt(easy, CURLOPT_WRITEDATA, this);
        curl_easy_setopt(easy, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(easy, CURLOPT_ERRORBUFFER, error);
        curl_easy_setopt(easy, CURLOPT_PRIVATE, this);
        if (progress_func) {
            curl_easy_setopt(easy, CURLOPT_NOPROGRESS, 0L);
            curl_easy_setopt(easy, CURLOPT_XFERINFOFUNCTION, libcurl_callbacks::on_progress);
            curl_easy_setopt(easy, CURLOPT_XFERINFODATA, this);
        }
        curl_easy_setopt(easy, CURLOPT_LOW_SPEED_TIME, 3L);
        curl_easy_setopt(easy, CURLOPT_LOW_SPEED_LIMIT, 10L);

        /* call this function to get a socket */
        curl_easy_setopt(easy, CURLOPT_OPENSOCKETFUNCTION, libcurl_callbacks::on_open_socket);
        curl_easy_setopt(easy, CURLOPT_OPENSOCKETDATA, channel.impl);

        /* call this function to close a socket */
        curl_easy_setopt(easy, CURLOPT_CLOSESOCKETFUNCTION, libcurl_callbacks::on_close_socket);
        curl_easy_setopt(easy, CURLOPT_CLOSESOCKETDATA, channel.impl);

    }
};


curl_session::curl_session(curl_channel & channel, receive_callback rcb, progress_callback pcb) :
    impl(new impl_t(channel, rcb, pcb)) {
}


void curl_session::set_url(char const * url) {
    assert(impl->url == nullptr);
    impl->channel.open();
    impl->url = strdup(url);
    curl_easy_setopt(impl->easy, CURLOPT_URL, url);
    fprintf(stdout,
            "\nAdding easy %p to multi %p (%s)", impl->easy, impl->channel.impl->multi, url);
    auto rc = curl_multi_add_handle(impl->channel.impl->multi, impl->easy);
    mcode_or_die("new_conn: curl_multi_add_handle", rc);

    /* note that the add_handle() will set a time-out to trigger very soon so
       that the necessary socket_action() call will be called by this app */
}


/* Update the event timer after curl_multi library calls */
int libcurl_callbacks::on_change_timeout(CURLM *multi, long timeout_ms, void * _chimpl)
{
    fprintf(stdout, "\nmulti_timer_cb: timeout_ms %ld", timeout_ms);

    chimpl_t * impl = reinterpret_cast<chimpl_t *>(_chimpl);

    /* cancel running timer */
    impl->timeout.cancel();

    if (timeout_ms > 0) {
        /* update timer */
        impl->timeout.expires_from_now(boost::posix_time::millisec(timeout_ms));
        impl->timeout.async_wait(std::bind(&chimpl_t::on_timeout, impl, _1));
    } else {
        /* call timeout function immediately */
        error_code error; /*success*/
        impl->on_timeout(error);
    }

    return 0;
}


/* Check for completed transfers, and remove their easy handles */
void curl_channel::impl_t::check_multi_info() {

    char *eff_url;
    CURLMsg *msg;
    int msgs_left;
    curl_session::impl_t * simpl;
    CURL *easy;
    CURLcode res;

    fprintf(stdout, "\nREMAINING: %d", still_running);

    while ((msg = curl_multi_info_read(multi, &msgs_left))) {
        if (msg->msg == CURLMSG_DONE) {
            easy = msg->easy_handle;
            res = msg->data.result;
            curl_easy_getinfo(easy, CURLINFO_PRIVATE, &simpl);
            curl_easy_getinfo(easy, CURLINFO_EFFECTIVE_URL, &eff_url);
            fprintf(stdout, "\nDONE: %s => (%d) %s", eff_url, res, simpl->error);
            curl_multi_remove_handle(multi, easy);
            free(simpl->url);
            simpl->url = nullptr;
            //curl_easy_cleanup(easy);
            //impl->close();
        }
    }
}


/* Called by asio when there is an action on a socket */
void curl_channel::impl_t::on_socket_event(asio_socket_t *tcp_socket,
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
void curl_channel::impl_t::on_timeout(error_code const & error)
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
void curl_channel::impl_t::remove_socket(int *f) {
    fprintf(stdout, "\nremsock: ");

    if (f) {
        free(f);
    }
}


void curl_channel::impl_t::change_socket_action(int *fdp, curl_socket_t sock, CURL * easy, int act)
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


void curl_channel::impl_t::add_socket(curl_socket_t sock, CURL * easy, int action)
{
    /* fdp is used to store current action */
    int *fdp = (int *) calloc(sizeof(int), 1);

    change_socket_action(fdp, sock, easy, action);
    curl_multi_assign(multi, sock, fdp);
}


/* CURLMOPT_SOCKETFUNCTION */
int libcurl_callbacks::on_socket_update(CURL * easy, curl_socket_t sock, int what, void * _chimpl, void *sockp)
{
    fprintf(stdout, "\nsock_cb: socket=%d, what=%d, sockp=%p", sock, what, sockp);

    chimpl_t * chimpl = reinterpret_cast<chimpl_t *>(_chimpl);
    int *actionp = (int *) sockp;
    const char *whatstr[] = { "none", "IN", "OUT", "INOUT", "REMOVE"};

    fprintf(stdout,
          "\nsocket callback: sock=%d easy=%p what=%s ", sock, easy, whatstr[what]);

    if (what == CURL_POLL_REMOVE) {
        fprintf(stdout, "\n");
        chimpl->remove_socket(actionp);
    } else {
        if (!actionp) {
            fprintf(stdout, "\nAdding data: %s", whatstr[what]);
            chimpl->add_socket(sock, easy, what);
        } else {
            fprintf(stdout,
                  "\nChanging action from %s to %s",
                  whatstr[*actionp], whatstr[what]);
            chimpl->change_socket_action(actionp, sock, easy, what);
        }
    }

    return 0;
}


/* CURLOPT_WRITEFUNCTION */
size_t libcurl_callbacks::on_receive_data(void * data, size_t size_per_record, size_t num_records, void * _session)
{
    size_t bytes_handled = 0;
    if (_session) {
        auto & session = reinterpret_cast<curl_session &>(*_session);
        auto func = session.impl->receive_func;
        if (func) {
            bytes_handled = func(session, data, size_per_record * num_records);
        }
    }
    return bytes_handled;
}

/* CURLOPT_XFERINFOFUNCTION */
int libcurl_callbacks::on_progress(void * _session, uint64_t expected_receive_total,
        uint64_t received_so_far, uint64_t expected_send_total,
        uint64_t sent_so_far)
{
    int err = 0;
    if (_session) {
        auto & session = reinterpret_cast<curl_session &>(*_session);
        auto func = session.impl->progress_func;
        if (func) {
            err = func(session, expected_receive_total, received_so_far,
                 expected_send_total, sent_so_far);
        }
    }
    return err;
}


/* CURLOPT_OPENSOCKETFUNCTION */
curl_socket_t libcurl_callbacks::on_open_socket(void * _chimpl, curlsocktype purpose,
                                curl_sockaddr *address)
{
    curl_socket_t sockfd = CURL_SOCKET_BAD;
    fprintf(stdout, "\nopensocket :");

    if (_chimpl) {
        auto & cdata = reinterpret_cast<chimpl_t &>(*_chimpl);
        if (purpose == CURLSOCKTYPE_IPCXN && address->family == AF_INET)
        {
            /* create a tcp socket object */
            asio_socket_t *tcp_socket = new asio_socket_t(cdata.io_service);

            /* open it and get the native handle*/
            error_code ec;
            tcp_socket->open(asio::ip::tcp::v4(), ec);

            if (ec) {
                fprintf(stdout, "\nERROR %d: %s. Returning CURL_SOCKET_BAD to signal error.", ec.value(), ec.message().c_str());
            } else {
                sockfd = tcp_socket->native_handle();
                fprintf(stdout, "\nOpened socket %d", sockfd);

                /* save it for monitoring */
                cdata.socket_map.insert(socket_pair_t(sockfd, tcp_socket));
            }
        }
    }

    return sockfd;
}



/* CURLOPT_CLOSESOCKETFUNCTION */
int libcurl_callbacks::on_close_socket(void * _chimpl, curl_socket_t item)
{
    fprintf(stdout, "\nclosesocket : %d", item);

    if (_chimpl) {
        auto & socket_map = reinterpret_cast<chimpl_t *>(_chimpl)->socket_map;
        auto it = socket_map.find(item);

        if (it != socket_map.end()) {
            delete it->second;
            socket_map.erase(it);
        }
    }

    return 0;
}


/* Die if we get a bad CURLMcode somewhere */
static void mcode_or_die(const char *where, CURLMcode code)
{
  if (CURLM_OK != code)
  {
    const char *sock;
    switch(code)
    {
    case CURLM_CALL_MULTI_PERFORM:
      sock = "CURLM_CALL_MULTI_PERFORM";
      break;
    case CURLM_BAD_HANDLE:
      sock = "CURLM_BAD_HANDLE";
      break;
    case CURLM_BAD_EASY_HANDLE:
      sock = "CURLM_BAD_EASY_HANDLE";
      break;
    case CURLM_OUT_OF_MEMORY:
      sock = "CURLM_OUT_OF_MEMORY";
      break;
    case CURLM_INTERNAL_ERROR:
      sock = "CURLM_INTERNAL_ERROR";
      break;
    case CURLM_UNKNOWN_OPTION:
      sock = "CURLM_UNKNOWN_OPTION";
      break;
    case CURLM_LAST:
      sock = "CURLM_LAST";
      break;
    default:
      sock = "CURLM_unknown";
      break;
    case CURLM_BAD_SOCKET:
      sock = "CURLM_BAD_SOCKET";
      fprintf(stdout, "\nERROR: %s returns %s", where, sock);
      /* ignore this error */
      return;
    }

    fprintf(stdout, "\nERROR: %s returns %s", where, sock);

    exit(code);
  }
}


}}} // end namespace
