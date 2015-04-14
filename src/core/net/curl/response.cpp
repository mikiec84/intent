#include <thread>

#include "core/net/curl/.private/response-impl.h"
#include "core/net/curl/response.h"
#include "core/util/monotonic_id.h"



using std::atomic;
using std::map;
using std::string;

using namespace std::placeholders;

namespace intent {
namespace core {
namespace net {
namespace curl {


uint32_t get_next_response_id() {
    static util::monotonic_id<uint32_t> the_generator;
    return the_generator.next();
}


#if 0
response::impl_t::impl_t(class response * resp, class request && req,
    receive_callback receive_func, progress_callback progress_func):
        id(get_next_id()),
        response(resp),
        request(),
        curl(nullptr),
        receive_func(receive_func),
        progress_func(progress_func),
        received_bytes(nullptr),
        allocated_byte_count(0),
        received_byte_count(0),
        expected_receive_total(0),
        sent_byte_count(0),
        expected_send_total(0),
        status_code(0),
        detached(false) {

    curl = curl_easy_init();
    printf("Called curl_easy_init() in %s, %s line %d; got 0x%p\n", __func__, __FILE__, __LINE__, curl);
    fflush(stdout);
    if (!curl) {
        fprintf(stdout, "\ncurl_easy_init() failed, exiting!");
        exit(2);
    }

    auto s = request.impl->session;
    auto ch = s->impl->channel;

    curl_easy_setopt(curl, CURLOPT_URL, request.get_url());
    set_curl_verb(curl, request.get_verb());
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
#ifdef DO_TIMEOUT
    curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, 3L);
    curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, 10L);
#endif

    /* call this function to get a socket */
    curl_easy_setopt(curl, CURLOPT_OPENSOCKETFUNCTION, libcurl_callbacks::on_open_socket);
    curl_easy_setopt(curl, CURLOPT_OPENSOCKETDATA, ch->impl);

    /* call this function to close a socket */
    curl_easy_setopt(curl, CURLOPT_CLOSESOCKETFUNCTION, libcurl_callbacks::on_close_socket);
    curl_easy_setopt(curl, CURLOPT_CLOSESOCKETDATA, ch->impl);

    ch->open();
    fprintf(stdout,
            "\nAdding curl %p to channel %u (%s)", curl, ch->get_id(), request.get_url());
    auto rc = curl_multi_add_handle(ch->impl->multi, curl);
    mcode_or_die("new_conn: multi_add_handle", rc);

    /* note that the add_handle() will set a time-out to trigger very soon so
       that the necessary socket_action() call will be called by this app */

}
#endif

response::response(session * s) :
        impl(new impl_t(s)) {
}


response::response(response && other) :
        impl(std::move(other.impl)) {
    other.impl = nullptr;
}


response::response(response const & other) :
        impl(other.impl) {
    impl->add_ref();
}


response::response(impl_t * other) :
        impl(other) {
    impl->add_ref();
}


response::~response() {
    if (impl) {
        impl->release_ref();
    }
}


uint16_t response::get_status_code() const {
    return impl->status_code;
}


void response::wait(struct timeout const & timeout) {
    fprintf(stderr, "entering %s()\n", __func__);
    std::this_thread::sleep_for(std::chrono::milliseconds(500000));
}


session & response::get_session() {
    return *impl->session;
}


session const & response::get_session() const {
    return *impl->session;
}


channel & response::get_channel() {
    return *impl->session->impl->channel;
}


channel const & response::get_channel() const {
    return *impl->session->impl->channel;
}


uint32_t response::get_id() const {
    return impl->id;
}


headers const & response::get_headers() const {
    return impl->headers;
}


string const & response::get_body() const {
    return impl->received_bytes;
}


static constexpr size_t max_stored_response_size = 16 * 1024 * 1024;

uint64_t response::impl_t::store_bytes(void * bytes, uint64_t byte_count) {

    if (byte_count) {

        auto received_byte_count = received_bytes.size();
        if (received_byte_count >= max_stored_response_size) {
            return 0;
        }

        auto desired_byte_count = received_byte_count + byte_count + 1/*null terminate*/;
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

        if (new_size > received_bytes.capacity()) {
            received_bytes.reserve(new_size);
        }
        received_bytes.append(reinterpret_cast<char const *>(bytes), byte_count);
    }

    fprintf(stderr, "stored %u bytes\n", (unsigned)byte_count);
    return byte_count;
}


uint64_t response::impl_t::store_header(void * bytes, uint64_t byte_count) {

    auto txt = reinterpret_cast<char const *>(bytes);
    if (byte_count) {
        headers.add(text::str_view(txt, byte_count));
    }

    fprintf(stderr, "stored %s\n", txt);
    return byte_count;
}


int response::impl_t::update_progress(uint64_t _expected_receive_total,
        uint64_t _received_so_far, uint64_t _expected_send_total, uint64_t _sent_so_far) {
    fprintf(stderr, "entering %s()\n", __func__);
    expected_receive_total = _expected_receive_total;
    expected_send_total = _expected_send_total;
    sent_byte_count = _sent_so_far;
    return 0;
}

}}}} // end namespace
