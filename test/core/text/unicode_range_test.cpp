#include "core/text/unicode_range.h"

#include "gtest/gtest.h"

using namespace intent::core::text;


TEST(unicode_range_test, no_such_value) {
    EXPECT_EQ(nullptr, get_unicode_range_info(0));
    EXPECT_EQ(nullptr, get_unicode_range_info(0xffffffff));
}


TEST(unicode_range_test, various_valid_values) {
    auto ascii = get_unicode_range_info('a');
    EXPECT_STREQ("Basic Latin", ascii->name);
    auto devanagari = get_unicode_range_info(0x0903);
    EXPECT_STREQ("Devanagari", devanagari->name);
}
