#include <thread>

#include "core/net/curl/.private/response-impl.h"
#include "core/net/curl/response.h"
#include "core/util/monotonic_id.h"



using std::atomic;
using std::map;
using std::string;
using std::mutex;
using std::lock_guard;

using namespace std::placeholders;

namespace intent {
namespace core {
namespace net {
namespace curl {


uint32_t get_next_response_id() {
    static util::monotonic_id<uint32_t> the_generator;
    return the_generator.next();
}


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


bool response::wait(unsigned timeout_millisecs) {
    // It's easier to implement wait() correctly in session impl, so delegate...
    return impl->session->impl->wait(timeout_millisecs);
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

    {
        auto simpl = session->impl;
        lock_guard<mutex> lock(simpl->mtx);
        if (simpl->state == session_state::requesting || simpl->state == session_state::waiting_for_response) {
            simpl->state = session_state::accepting_response;
        }
    }
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
    } else {
    }

    return byte_count;
}


uint64_t response::impl_t::store_header(void * bytes, uint64_t byte_count) {

    auto txt = reinterpret_cast<char const *>(bytes);
    if (byte_count) {
        auto captured_byte_count = byte_count - 2;
        // Curl calls us with a final "header" which is just a blank line. We
        // don't want to bother to store this header.
        if (byte_count >= 2 && txt[byte_count - 2] == '\r' && txt[byte_count - 1] == '\n') {
            if (byte_count == 2) {
                return byte_count;
            }
        } else {
            // Unlikely. Got a malformed header that didn't end in CRLF. Handle
            // as gracefully as we can.
            captured_byte_count += 2;
        }
        headers.add(text::str_view(txt, captured_byte_count));
    } else {
        fprintf(stderr, "got zero-byte header\n");
    }

    return byte_count;
}


bool response::impl_t::update_progress(uint64_t _expected_receive_total,
        uint64_t _received_so_far, uint64_t _expected_send_total,
        uint64_t _sent_byte_count) {

    bool change_detected = false;
    #define update_item(x) if (_##x != x) { change_detected = true; x = _##x; }
    update_item(expected_receive_total);
    update_item(expected_receive_total);
    update_item(expected_send_total);
    update_item(sent_byte_count);
    #undef update_item

    return change_detected;
}

}}}} // end namespace
