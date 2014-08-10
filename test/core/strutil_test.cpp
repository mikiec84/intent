#include "core/countof.h"
#include "core/sslice.h"
#include "core/strutil.h"

#include "gtest/gtest.h"

using std::string;

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

TEST(strutil_test, split) {
    char const * const TXT = "this is a test, I think";
    char const * y[] = {"this", "is", "a", "test", "I", "think"};
    auto n = countof(y);

    auto x = split<string>(TXT, ", ");
    EXPECT_EQ(n, x.size());
    for (unsigned i = 0; i < n; ++i) {
        EXPECT_STREQ(y[i], x[i].c_str());
    }

    auto z = split<sslice>(TXT, ", ");
    EXPECT_EQ(n, z.size());
    for (unsigned i = 0; i < n; ++i) {
        EXPECT_EQ(0, strcmp(y[i], z[i]));
    }
}

TEST(strutil_test, split_with_null_or_empty_input) {
    EXPECT_EQ(0U, split<string>("", "aeiou").size());
    EXPECT_EQ(0U, split<sslice>(nullptr, "aeiou").size());
}
