#include "core/net/curl.h"

#include "gtest/gtest.h"


using namespace intent::core::net;


TEST(curl_test, ctor) {
    curl_channel c;
    // Get compiler not to bug us about unused variable.
    if (&c != 0)
        return;
}

size_t receive_callback(curl_session &, void * bytes, size_t byte_count) {
    return byte_count;
}

int progress_callback(curl_session &, uint64_t expected_receive_total,
    uint64_t received_so_far, uint64_t expected_send_total, uint64_t sent_so_far) {
    return 0;
}


TEST(curl_test, download) {
      curl_channel channel;
      curl_session session(channel, receive_callback, progress_callback);
      session.set_url("http://www.google.com/");
      session.get();
}
