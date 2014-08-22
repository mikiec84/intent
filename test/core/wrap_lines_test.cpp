#include "core/sslice.h"
#include "core/wrap_lines.h"

#include "gtest/gtest.h"

using namespace intent::core;

#define LONG_LINE \
"The quick brown fox jumped over the lazy red dog--and boy, was he surprised when that happened!"
#define SUPER_LONG_LINE LONG_LINE " " LONG_LINE " " LONG_LINE " " LONG_LINE

TEST(wrap_lines_test, easy) {
    // We should get a wrap after the hyphen.
    auto wrapped = wrap_lines("123 567 9 12345", 10);
    ASSERT_STREQ("123 567 9\n12345", wrapped.c_str());
}

TEST(wrap_lines_test, big) {
    auto wrapped = wrap_lines(SUPER_LONG_LINE);
    ASSERT_STREQ(
"The quick brown fox jumped over the lazy red dog--and boy, was he surprised\n"
"when that happened! The quick brown fox jumped over the lazy red dog--and boy,\n"
"was he surprised when that happened! The quick brown fox jumped over the lazy\n"
"red dog--and boy, was he surprised when that happened! The quick brown fox\n"
"jumped over the lazy red dog--and boy, was he surprised when that happened!",
                wrapped.c_str());
}

TEST(wrap_lines_test, empty) {
    auto wrapped = wrap_lines("");
    ASSERT_STREQ("", wrapped.c_str());
}

TEST(wrap_lines_test, nullptr) {
    auto wrapped = wrap_lines(nullptr);
    ASSERT_STREQ("", wrapped.c_str());
}

TEST(wrap_lines_test, after_hyphen) {
    // We should get a wrap after the hyphen.
    auto wrapped = wrap_lines("123456 8-12345", 10);
    ASSERT_STREQ("123456 8-\n12345", wrapped.c_str());
}

TEST(wrap_lines_test, after_double_hyphen) {
    // Should get a wrap after the double hyphen.
    auto wrapped = wrap_lines("1234 67--12345", 10);
    ASSERT_STREQ("1234 67--\n12345", wrapped.c_str());
}

TEST(wrap_lines_test, between_double_hyphen) {
    // Shouldn't get a wrap in the middle of a double hyphen.
    auto wrapped = wrap_lines("12345 78--12345", 10);
    ASSERT_STREQ("12345\n78--12345", wrapped.c_str());
}

TEST(wrap_lines_test, between_double_hyphen2) {
    // Shouldn't get a wrap in the middle of a double hyphen.
    auto wrapped = wrap_lines("12345 78--123456", 10);
    ASSERT_STREQ("12345\n78--\n123456", wrapped.c_str());
}

TEST(wrap_lines_test, between_triple_hyphens) {
    // Anything longer than double hyphen should be breakable anywhere--but we
    // prefer to break as late as possible.
    auto wrapped = wrap_lines("1234567---12345", 10);
    ASSERT_STREQ("1234567--\n-12345", wrapped.c_str());
}
