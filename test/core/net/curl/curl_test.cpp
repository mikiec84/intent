#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#include <atomic>

#include "core/net/curl/session_state.h"
#include "core/net/curl/request.h"
#include "core/net/curl/response.h"

#include "gtest/gtest.h"


// Prevent compiler from complaining about unused variables.
#define use_variable(x) if (&x == nullptr) return


using namespace intent::core::net::curl;

size_t receive_cb(request &, void * bytes, size_t byte_count) {
    fwrite(bytes, 1, byte_count, stdout);
    return byte_count;
}


int progress_cb(request & req, uint64_t expected_receive_total,
    uint64_t received_so_far, uint64_t expected_send_total, uint64_t sent_so_far) {
    fprintf(stderr, "receive progress on request %u: %" PRIu64 " of %" PRIu64, req.get_id(), received_so_far, expected_receive_total);
    fprintf(stderr, "send progress on request %u: %" PRIu64" of %" PRIu64, req.get_id(), sent_so_far, expected_send_total);
    return 0;
}


TEST(curl_test, channel_lifecycle) {
    // Prove that we can setup and teardown a channel without problems.
    channel c;
    use_variable(c);
}


TEST(curl_test, session_lifecycle_normal) {
    // Prove that we can setup and teardown a session without problems.
    channel c;
    session s(c);
    use_variable(s);
}


TEST(curl_test, atomic_session_state_is_lock_free) {
    std::atomic<session_state> x(session_state::idle);
    ASSERT_TRUE(x.is_lock_free());
}


TEST(curl_test, easiest_download_to_debug) {
    session s;
    request req = s.get_current_request();
    req.set_url("http://127.0.0.1:5006/stats");
    req.set_verb("get");
    req.get_headers().set("User-Agent", "intent.core.net.curl");
    auto resp = s.send();

    resp.wait();
    ASSERT_EQ(200, resp.get_status_code());
    ASSERT_FALSE(resp.get_headers().is_empty());
    ASSERT_TRUE(resp.get_body().size() > 0);
}


TEST(curl_test, simplest_download) {
    auto resp = request::get("http://www.google.com/");
    ASSERT_EQ(200, resp.get_status_code());
    ASSERT_FALSE(resp.get_headers().is_empty());
    ASSERT_TRUE(resp.get_body().size() > 0);
}

