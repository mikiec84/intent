#include "core/net/http_method.h"

#include "gtest/gtest.h"


using namespace intent::core::net;


TEST(http_method_test, all) {

	auto i = get_http_method_count();
	ASSERT_TRUE(i > 5);

	while (true) {
		--i;
		http_method const * m1 = get_http_method_by_index(i);
		if (m1 == nullptr) {
			ADD_FAILURE() << "get_http_method_by_index(" << i << ") returned null.";
		} else {
			auto m2 = get_http_method_by_id(m1->id);
			if (m2 != m1) {
				ADD_FAILURE() << "get_http_method_by_id(" << m1->id << ") != get_http_method_by_index(" << i << ").";
			} else {
				auto m3 = get_http_method_by_name(m2->verb);
				if (m2 != m3) {
					ADD_FAILURE() << "get_http_method_by_name(" << m2->verb << ") != get_http_method_by_index(" << i << ").";
				} else {
					auto m4 = get_http_method_by_name(m3->id_str);
					if (m4 != m3) {
						ADD_FAILURE() << "get_http_method_by_name(" << m3->id_str << ") != get_http_method_by_index(" << i << ").";
					}
				}
			}
		}

		if (i == 0) {
			break;
		}
	}
}

TEST(http_method_test, bad_lookups) {

	ASSERT_EQ(nullptr, get_http_method_by_name(nullptr));
	ASSERT_EQ(nullptr, get_http_method_by_name(""));
	ASSERT_EQ(nullptr, get_http_method_by_name("bad"));

	ASSERT_EQ(nullptr, get_http_method_by_index(10000u));
#ifndef DEBUG // don't test this in debug mode; the clang sanitizer will complain
	ASSERT_EQ(nullptr, get_http_method_by_id(static_cast<http_method_id>(10000u)));
#endif
}
