#include "core/strutil.h"

#include "gtest/gtest.h"

#define try_trim_ex(trimmer, cstr, expected, chars) \
    { char const * end = strchr(cstr, 0); \
    EXPECT_STREQ(expected, trimmer(cstr, end, chars)); }

#define try_trim(trimmer, cstr, expected) \
    try_trim_ex(trimmer, cstr, expected, ANY_WHITESPACE)

TEST(strutil_test, ltrim) {
    try_trim(ltrim, " \ttest", "test");
    try_trim(ltrim, "test ", "test ");
    try_trim_ex(ltrim, "test ", "st ", "et");
    try_trim(ltrim, "\r\n \t ", "");
}

TEST(strutil_test, rtrim) {
    try_trim(rtrim, "test \t", " \t");
    try_trim(rtrim, " test", "");
    try_trim_ex(rtrim, " test", "st", "st");
    try_trim(rtrim, "\r\n \t ", "\r\n \t ");
    try_trim(rtrim, "s ", " ");
}
