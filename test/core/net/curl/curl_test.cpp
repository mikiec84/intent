#include "core/net/curl/request.h"
#include "core/net/curl/response.h"

#include "gtest/gtest.h"


using namespace intent::core::net::curl;

size_t receive_cb(request &, void * bytes, size_t byte_count) {
	fwrite(bytes, 1, byte_count, stdout);
	return byte_count;
}


int progress_cb(request & req, uint64_t expected_receive_total,
	uint64_t received_so_far, uint64_t expected_send_total, uint64_t sent_so_far) {
	fprintf(stderr, "receive progress on request %u: %lu of %lu", req.get_id(), received_so_far, expected_receive_total);
	fprintf(stderr, "send progress on request %u: %lu of %lu", req.get_id(), sent_so_far, expected_send_total);
	return 0;
}


TEST(curl_test, simple_download) {
	channel c;
	session s(c);
	response r = s.get("http://www.google.com/");
	// Get compiler not to bug us about unused variable.
	if (&s != 0)
		return;
}

