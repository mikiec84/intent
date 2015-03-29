#include <atomic>
#include <thread>

#include <asio.hpp>

#include "core/net/curl/.private/channel-impl.h"
#include "core/net/curl/.private/libcurl_callbacks.h"
#include "core/net/curl/.private/request-impl.h"
#include "core/net/curl/.private/response-impl.h"
#include "core/net/curl/.private/session-impl.h"
#include "core/net/curl/response.h"


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
namespace curl {


typedef asio::ip::tcp::socket asio_socket_t;
typedef map<curl_socket_t, asio_socket_t *> socket_map_t;
typedef std::pair<curl_socket_t, asio_socket_t *> socket_pair_t;


static uint32_t get_next_id() {
    static atomic<uint32_t> the_next_id;
    return the_next_id.fetch_add(1);
}


response::impl_t::impl_t(class response * resp, class request && req,
    receive_callback receive_func, progress_callback progress_func):
        id(get_next_id()),
        response(resp),
        request(std::move(req)),
        curl(curl_easy_init()),
        receive_func(receive_func),
        progress_func(progress_func),
        received_bytes(nullptr),
        allocated_byte_count(0),
        received_byte_count(0),
        expected_receive_total(0),
        sent_byte_count(0),
        expected_send_total(0) {

    curl = curl_easy_init();
    if (!curl) {
        fprintf(stdout, "\ncurl_easy_init() failed, exiting!");
        exit(2);
    }

    auto s = request.impl->session;
    auto ch = s->impl->channel;

    s->register_response(response);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, libcurl_callbacks::on_receive_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error);
    curl_easy_setopt(curl, CURLOPT_PRIVATE, this);
    if (progress_func) {
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
        curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, libcurl_callbacks::on_progress);
        curl_easy_setopt(curl, CURLOPT_XFERINFODATA, this);
    }
    curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, 3L);
    curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, 10L);

    /* call this function to get a socket */
    curl_easy_setopt(curl, CURLOPT_OPENSOCKETFUNCTION, libcurl_callbacks::on_open_socket);
    curl_easy_setopt(curl, CURLOPT_OPENSOCKETDATA, ch.impl);

    /* call this function to close a socket */
    curl_easy_setopt(curl, CURLOPT_CLOSESOCKETFUNCTION, libcurl_callbacks::on_close_socket);
    curl_easy_setopt(curl, CURLOPT_CLOSESOCKETDATA, ch.impl);

    ch.open();
    curl_easy_setopt(curl, CURLOPT_URL, request.get_url());
    fprintf(stdout,
            "\nAdding curl %p to channel %u (%s)", curl, ch.get_id(), request.get_url());
    auto rc = curl_multi_add_handle(ch.impl->multi, curl);
    mcode_or_die("new_conn: curl_multi_add_handle", rc);

    /* note that the add_handle() will set a time-out to trigger very soon so
       that the necessary socket_action() call will be called by this app */

}


response::impl_t::~impl_t() {
    request.impl->session->register_response(response, false);
}


response::response(request && req, receive_callback rcb, progress_callback pcb) :
        impl(new impl_t(this, std::move(req), rcb, pcb)) {
}


response::~response() {
    delete impl;
}


response::response(response && other) : impl(std::move(other.impl)) {
    impl->response = this;
}


channel & response::get_channel() {
    return impl->request.impl->session->get_channel();
}


channel const & response::get_channel() const {
    return impl->request.impl->session->get_channel();
}


uint32_t response::get_id() const {
    return impl->id;
}


headers const & response::get_headers() const {
    return impl->headers;
}


headers & response::get_headers() {
    return impl->headers;
}


uint16_t response::get_response_code() const {
    return 0;
}


static constexpr size_t max_stored_response_size = 16 * 1024 * 1024;

uint64_t response::store_bytes_in_response(response & r, void * bytes, uint64_t byte_count) {

    if (byte_count) {

        auto i = r.impl;
        if (i->allocated_byte_count >= max_stored_response_size) {
            return 0;
        }

        auto desired_byte_count = i->received_byte_count + byte_count + 1/*null terminate*/;
        auto new_size = desired_byte_count;
        if (new_size > max_stored_response_size) {
            // We are going to store a subset of the bytes we were given. How
            // many bytes is that?
            byte_count -= (new_size - max_stored_response_size);
            new_size = max_stored_response_size;
        } else {
            auto round_up_to = new_size < 8192 ? 1024 : 4096;
            auto mod = new_size % round_up_to;
            if (mod) {
                new_size += round_up_to - mod;
            }
        }

        if (new_size > i->allocated_byte_count) {

            auto new_ptr = realloc(i->received_bytes, new_size);
            if (!new_ptr) {
                return 0;
            }

            i->received_bytes = reinterpret_cast<char *>(new_ptr);
            i->allocated_byte_count = new_size;
            memcpy(i->received_bytes + i->received_byte_count, bytes, byte_count);
        } else {
            memcpy(i->received_bytes, bytes, byte_count);
        }
        i->received_byte_count += byte_count;
        i->received_bytes[i->received_byte_count] = 0;
    }

    return byte_count;
}


int response::update_progress_in_response(response & r, uint64_t expected_receive_total,
        uint64_t received_so_far, uint64_t expected_send_total, uint64_t sent_so_far) {
    auto i = r.impl;
    i->expected_receive_total = expected_receive_total;
    i->expected_send_total = expected_send_total;
    if (i->receive_func != store_bytes_in_response) {
        i->received_byte_count = received_so_far;
    }
    i->sent_byte_count = sent_so_far;
    return 0;
}


}}}} // end namespace
