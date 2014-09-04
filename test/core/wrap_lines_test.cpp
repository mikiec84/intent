#include "core/sslice.h"
#include "core/wrap_lines.h"

#include "gtest/gtest.h"

using namespace intent::core::text;

#define LONG_LINE \
"The quick brown fox jumped over the lazy red dog--and boy, was he surprised when that happened!"
#define SUPER_LONG_LINE LONG_LINE " " LONG_LINE " " LONG_LINE " " LONG_LINE

TEST(wrap_lines_test, easy) {
    // We should get a wrap after the hyphen.
    auto wrapped = wrap_lines("123 567 9 12345", 10);
    ASSERT_STREQ("123 567 9\n12345", wrapped.c_str());
}

TEST(wrap_lines_test, easy_crlf) {
    // We should get a wrap after the hyphen.
    auto wrapped = wrap_lines("123 567 9 12345", 10, "\r\n");
    ASSERT_STREQ("123 567 9\r\n12345", wrapped.c_str());
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

TEST(wrap_lines_test, too_long) {
    // We should get a wrap in the middle of the string if we have no better
    // options.
    auto wrapped = wrap_lines("123456789012345", 10);
    ASSERT_STREQ("123456789\n012345", wrapped.c_str());
}

TEST(wrap_lines_test, after_double_hyphen) {
    // Should get a wrap after the double hyphen.
    auto wrapped = wrap_lines("1234 67--12345", 10);
    ASSERT_STREQ("1234 67--\n12345", wrapped.c_str());
}

TEST(wrap_lines_test, between_double_hyphen) {
    // Should get a wrap before double hyphen, because we can't wrap in the
    // middle of one.
    auto wrapped = wrap_lines("12345 78--12345", 10);
    ASSERT_STREQ("12345\n78--12345", wrapped.c_str());
}

TEST(wrap_lines_test, between_double_hyphen2) {
    // Should get a wrap before double hyphen, as in previous test. We should
    // also get one after as well, because 10 chars remain on the 2nd line.
    auto wrapped = wrap_lines("12345 78--123456", 10);
    ASSERT_STREQ("12345\n78--\n123456", wrapped.c_str());
}

TEST(wrap_lines_test, between_triple_hyphens) {
    // Anything longer than double hyphen should be breakable anywhere--but we
    // prefer to break as late as possible.
    auto wrapped = wrap_lines("1234567---12345", 10);
    ASSERT_STREQ("1234567--\n-12345", wrapped.c_str());
}

TEST(wrap_lines_test, exactly_enough) {
    // When wrapping at 20 chars, we have 19 printable columns plus the place
    // for CR+LF, so no wrapping should happen.
    auto wrapped = wrap_lines("1234567890123456789", 20);
    ASSERT_STREQ("1234567890123456789", wrapped.c_str());
}

TEST(wrap_lines_test, final_char_is_space) {
    // Anything longer than double hyphen should be breakable anywhere--but we
    // prefer to break as late as possible.
    auto wrapped = wrap_lines("1234567890123456789 ", 20);
    ASSERT_STREQ("1234567890123456789\n", wrapped.c_str());
}

TEST(wrap_lines_test, inconsistent_line_breaks) {
    auto wrapped = wrap_lines("1234 6789\r\n1234\n1234\rabcd efgh ijkl", 10);
    ASSERT_STREQ("1234 6789\n1234\n1234\nabcd efgh\nijkl", wrapped.c_str());
}

TEST(wrap_lines_test, indent_without_break) {
    auto wrapped = wrap_lines("       890123", 10);
    ASSERT_STREQ("       89\n       01\n       23", wrapped.c_str());
}

TEST(wrap_lines_test, indents) {
    auto wrapped = wrap_lines("  abc\n    def89 ghi", 10);
    ASSERT_STREQ("  abc\n    def89\n    ghi", wrapped.c_str());
}

TEST(wrap_lines_test, multibyte_char) {
    // This is a short Chinese sentence. Each visible glyph takes 3 bytes to
    // represent. The test proves that A) we never break in the middle of a
    // multibyte char; and B) we count advances by logical (visible) glyph
    // instead of by byte.
    auto wrapped = wrap_lines(
            "\xe6\x88\x91"
            "\xe8\xae\xa4"
            "\xe4\xb8\xba" // if we don't advance right, we'll break after this.
            "\xe8\xae\xa1"
            "\xe7\xae\x97"
            "\xe6\x9c\xba"
            "\xe6\x98\xaf"
            "\xe5\xa4\x8d"
            "\xe6\x9d\x82" // last visible glyph of first line
            "\xe7\x9a\x84" // first glyph of wrapped line
            "\xef\xbc\x8c"
            "\xe4\xbd\x86"
            "\xe6\x9c\x89"
            "\xe8\xb6\xa3", 10);
    ASSERT_STREQ(
            "\xe6\x88\x91"
            "\xe8\xae\xa4"
            "\xe4\xb8\xba"
            "\xe8\xae\xa1"
            "\xe7\xae\x97"
            "\xe6\x9c\xba"
            "\xe6\x98\xaf"
            "\xe5\xa4\x8d"
            "\xe6\x9d\x82"
            "\n"
            "\xe7\x9a\x84"
            "\xef\xbc\x8c"
            "\xe4\xbd\x86"
            "\xe6\x9c\x89"
            "\xe8\xb6\xa3", wrapped.c_str());
}
