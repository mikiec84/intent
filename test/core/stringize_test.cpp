#include "core/stringize.h"

#include "gtest/gtest.h"

#define MY_FIELD_WIDTH 17

TEST(stringize_test, single_stringize) {
    ASSERT_STREQ("MY_FIELD_WIDTH", STRINGIZE(MY_FIELD_WIDTH));
    ASSERT_STREQ("x", STRINGIZE(x));
}

TEST(stringize_test, double_stringize) {
    ASSERT_STREQ("17", DOUBLE_STRINGIZE(MY_FIELD_WIDTH));
    ASSERT_STREQ("x", DOUBLE_STRINGIZE(x));
}
