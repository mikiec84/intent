#include "core/countof.h"
#include "core/interp.h"

#include "gtest/gtest.h"

TEST(interp_test, null_arg) {
    auto s = interp("hello, {1}.", {nullptr});
    EXPECT_STREQ("hello, (null).", s.c_str());
}

TEST(interp_test, no_args) {
    auto s = interp("hello", {});
    EXPECT_STREQ("hello", s.c_str());
}

TEST(interp_test, one_arg) {
    auto s = interp("hello, {1}", {"world"});
    EXPECT_STREQ("hello, world", s.c_str());
}

TEST(interp_test, one_arg_repeated) {
    auto s = interp("hello, {1} {1} {1}", {"world"});
    EXPECT_STREQ("hello, world world world", s.c_str());
}

TEST(interp_test, skipped_arg) {
    auto s = interp("hello, {2} {2} {2}", {0, "world"});
    EXPECT_STREQ("hello, world world world", s.c_str());
}

TEST(interp_test, two_digit_arg) {
    auto s = interp("hello, {10} {2}", {0, "world", 3, 4, 5, 6, 7, 8, 9, "ten"});
    EXPECT_STREQ("hello, ten world", s.c_str());
}

TEST(interp_test, three_digit_arg) {
    auto s = interp("hello, {200}.", {0, "world", 3, 4, 5, 6, 7, 8, 9, "ten"});
    EXPECT_STREQ("hello, {200}.", s.c_str());
}

TEST(interp_test, arg_with_comment) {
    auto s = interp("x {1 = label for variable}.", {"my label"});
    EXPECT_STREQ("x my label.", s.c_str());
}

TEST(interp_test, arg_with_format_specifier) {
    auto s = interp("{1%.2f} is an approximation of pi.", {3.1415926535});
    EXPECT_STREQ("3.14 is an approximation of pi.", s.c_str());
}

TEST(interp_test, pluralize) {
    char const * fmt = "Found {1} warning{1~s||s}.";
    int args[] = {
        0, 1, 2, 3
    };
    char const * expected[] = {
            "Found 0 warnings.",
            "Found 1 warning.",
            "Found 2 warnings.",
            "Found 3 warnings."
    };
    for (unsigned i = 0; i < countof(args); ++i) {
        auto s = interp(fmt, {args[i]});
        EXPECT_STREQ(expected[i], s.c_str());
    }
}
