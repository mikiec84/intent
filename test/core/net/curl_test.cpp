#include "core/net/curl.h"

#include "gtest/gtest.h"


using namespace intent::core::net;


TEST(curl_test, ctor) {
	curl_channel c;
	// Get compiler not to bug us about unused variable.
	if (&c != 0)
		return;
}

