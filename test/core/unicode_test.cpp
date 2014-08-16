#include "core/unicode.h"

#include "gtest/gtest.h"

using namespace intent::core;

TEST(unicode_test, is_utf8_lead_byte) {
    EXPECT_TRUE(is_utf8_lead_byte(static_cast<char>(0xc0)));
    EXPECT_TRUE(is_utf8_lead_byte(static_cast<char>(0xd8)));
    EXPECT_FALSE(is_utf8_lead_byte(static_cast<char>(0x80)));
    EXPECT_FALSE(is_utf8_lead_byte(static_cast<char>(0xa8)));
    EXPECT_FALSE(is_utf8_lead_byte('x'));
    EXPECT_FALSE(is_utf8_lead_byte(static_cast<char>(0)));
}

TEST(unicode_test, is_utf8_trail_byte) {
    EXPECT_FALSE(is_utf8_trail_byte(static_cast<char>(0xc0)));
    EXPECT_FALSE(is_utf8_trail_byte(static_cast<char>(0xd8)));
    EXPECT_TRUE(is_utf8_trail_byte(static_cast<char>(0x80)));
    EXPECT_TRUE(is_utf8_trail_byte(static_cast<char>(0xa8)));
    EXPECT_FALSE(is_utf8_trail_byte('x'));
    EXPECT_FALSE(is_utf8_trail_byte(static_cast<char>(0)));
}

TEST(unicode_test, find_codepoint_in_utf8) {
    EXPECT_STREQ(nullptr, find_codepoint_in_utf8(nullptr, 25));
    EXPECT_STREQ(nullptr, find_codepoint_in_utf8("abc", 25));
    EXPECT_STREQ("bc", find_codepoint_in_utf8("abc", 'b'));
    EXPECT_STREQ("\xC3\xB1ol", find_codepoint_in_utf8("espa\xC3\xB1ol", 0x00F1));
    EXPECT_STREQ("\xE5\xB0\x8D xyz", find_codepoint_in_utf8("abc \xE5\xB0\x8D xyz", 0x5C0D));
    // The following search is in invalid utf8. We should not match trail bytes.
    EXPECT_STREQ(nullptr, find_codepoint_in_utf8("abc \x88\xB0\x8D xyz", 0x88));
    EXPECT_STREQ(nullptr, find_codepoint_in_utf8("abc \x88\xB0\x8D xyz", 0xB0));
    EXPECT_STREQ(nullptr, find_codepoint_in_utf8("abc \x88\xB0\x8D xyz", 0x8D));
}

TEST(unicode_test, get_codepoint_from_utf8) {
    codepoint_t cp;
    EXPECT_STREQ(0, get_codepoint_from_utf8(nullptr, cp));
    EXPECT_STREQ("bc", get_codepoint_from_utf8("abc", cp));
    EXPECT_EQ(static_cast<codepoint_t>('a'), cp);
    EXPECT_STREQ(" XYZ", get_codepoint_from_utf8("\xC3\xB1 XYZ", cp));
    EXPECT_EQ(0x00F1U, cp);
    EXPECT_STREQ(" xyz", get_codepoint_from_utf8("\xE5\xB0\x8D xyz", cp));
    EXPECT_EQ(0x5C0DU, cp);
    EXPECT_STREQ("\xB0\x8D xyz", get_codepoint_from_utf8("\x85\xB0\x8D xyz", cp));
    EXPECT_EQ(UNICODE_REPLACEMENT_CHAR, cp);
    EXPECT_STREQ("\x8D xyz", get_codepoint_from_utf8("\xB0\x8D xyz", cp));
    EXPECT_EQ(UNICODE_REPLACEMENT_CHAR, cp);
    EXPECT_STREQ(" xyz", get_codepoint_from_utf8("\x8D xyz", cp));
    EXPECT_EQ(UNICODE_REPLACEMENT_CHAR, cp);
    EXPECT_STREQ("\xB0\x8D xyz", get_codepoint_from_utf8("\xFD\xB0\x8D xyz", cp));
    EXPECT_EQ(UNICODE_REPLACEMENT_CHAR, cp);
    EXPECT_STREQ("\xB0\x8D xyz", get_codepoint_from_utf8("\xF9\xB0\x8D xyz", cp));
    EXPECT_EQ(UNICODE_REPLACEMENT_CHAR, cp);
}

TEST(unicode_test, add_codepoint_to_utf8) {
    char buf[32];
    size_t buf_len = sizeof(buf);
    char * p = nullptr;

    EXPECT_FALSE(add_codepoint_to_utf8(p, buf_len, 'a'));

    #define CHECK_ADD(size, cp) \
        p = buf; \
        buf_len = sizeof(buf); \
        add_codepoint_to_utf8(p, buf_len, cp); \
        EXPECT_EQ(buf + size, p)

    CHECK_ADD(1, 'a');
    CHECK_ADD(2, 0x00F1);
    CHECK_ADD(3, 0x5C0D);

    #undef CHECK_ADD

    buf_len = 0;
    EXPECT_FALSE(add_codepoint_to_utf8(p = buf, buf_len, 'a'));
    EXPECT_FALSE(add_codepoint_to_utf8(p = buf, buf_len, 0x00F1));
    EXPECT_FALSE(add_codepoint_to_utf8(p = buf, buf_len, 0x5C0D));
}

TEST(unicode_test, cat_codepoint_to_utf8) {
    char buf[32];
    size_t buf_len = sizeof(buf);
    char * p = nullptr;

    EXPECT_FALSE(cat_codepoint_to_utf8(p, buf_len, 'a'));

    p = buf;
    cat_codepoint_to_utf8(p, buf_len, 'a');
    EXPECT_STREQ("a", buf);

    p = buf;
    cat_codepoint_to_utf8(p, buf_len, 0x00F1);
    EXPECT_STREQ("\xC3\xB1", buf);

    p = buf;
    cat_codepoint_to_utf8(p, buf_len, 0x5C0D);
    EXPECT_STREQ("\xE5\xB0\x8D", buf);

    p = buf;
    cat_codepoint_to_utf8(p, buf_len, 0x100010);
    EXPECT_STREQ("\xF4\x80\x80\x90", buf);

    p = buf;
    buf_len = 0;
    EXPECT_FALSE(cat_codepoint_to_utf8(p, buf_len, 'a'));

    buf_len = 1;
    EXPECT_FALSE(cat_codepoint_to_utf8(p, buf_len, 0x00F1));

    buf_len = 2;
    EXPECT_FALSE(cat_codepoint_to_utf8(p, buf_len, 0x5C0D));

    buf_len = 3;
    EXPECT_FALSE(cat_codepoint_to_utf8(p, buf_len, 0x100010));
}

TEST(unicode_test, scan_unicode_escape_sequence) {
    codepoint_t cp;
    EXPECT_STREQ(nullptr, scan_unicode_escape_sequence(nullptr, cp));

    #define CHECKSEQ(seq, expected_str, expected_cp) \
        EXPECT_STREQ(expected_str, scan_unicode_escape_sequence(seq, cp)); \
        EXPECT_EQ(static_cast<codepoint_t>(expected_cp), cp)

    CHECKSEQ("x252", "2", 0x25);
    CHECKSEQ("0470", "0", 39);
    CHECKSEQ("u25596", "6", 0x2559);
    CHECKSEQ("U0010FF859", "9", 0x0010FF85);
    CHECKSEQ("U0810FF859", "0810FF859", UNICODE_REPLACEMENT_CHAR);
    CHECKSEQ("n ", " ", '\n');
    CHECKSEQ("\" ", " ", '"');
    CHECKSEQ("' ", " ", '\'');

    #undef CHECKSEQ
}

TEST(unicode_test, add_utf8_or_escape_sequence) {
    char buf[32];
    size_t buf_len = sizeof(buf);
    char * p = nullptr;

    EXPECT_FALSE(add_utf8_or_escape_sequence(p, buf_len, 'a'));

    #define CHECK_ADD(size, cp) \
        p = buf; \
        buf_len = sizeof(buf); \
        add_utf8_or_escape_sequence(p, buf_len, cp); \
        EXPECT_EQ(buf + size, p)

    CHECK_ADD(1, 'a');
    CHECK_ADD(2, '"');
    CHECK_ADD(2, '\\');
    CHECK_ADD(2, '\n');
    CHECK_ADD(2, '\t');
    CHECK_ADD(2, '\n');
    CHECK_ADD(2, '\r');
    CHECK_ADD(2, '\r');
    CHECK_ADD(2, '\'');
    CHECK_ADD(2, '\a');
    CHECK_ADD(2, '\v');
    CHECK_ADD(4, 0x7F);
    CHECK_ADD(6, 0x00F1);
    CHECK_ADD(6, 0x5C0D);

    #undef CHECK_ADD

    buf_len = 0;
    EXPECT_FALSE(add_utf8_or_escape_sequence(p = buf, buf_len, 'a'));

    buf_len = 5;
    EXPECT_FALSE(add_utf8_or_escape_sequence(p = buf, buf_len, 0x00F1));
    EXPECT_FALSE(add_utf8_or_escape_sequence(p = buf, buf_len, 0x5C0D));

    buf_len = 9;
    EXPECT_FALSE(add_utf8_or_escape_sequence(p = buf, buf_len, 0x0010FF85));
}

TEST(unicode_test, cat_utf8_or_escape_sequence) {
    char buf[32];
    size_t buf_len = sizeof(buf);
    char * p = nullptr;

    EXPECT_FALSE(cat_utf8_or_escape_sequence(p, buf_len, 'a'));

    #define CHECKSEQ(expected, cp) \
        p = buf; \
        buf_len = sizeof(buf); \
        cat_utf8_or_escape_sequence(p, buf_len, cp); \
        EXPECT_STREQ(expected, buf)

    CHECKSEQ("a", 'a');
    CHECKSEQ("\\\"", '"');
    CHECKSEQ("\\n", '\n');
    CHECKSEQ("\\u00F1", 0x00F1);
    CHECKSEQ("\\u00F1", 0x00F1);
    CHECKSEQ("\\u00F1", 0x00F1);
    CHECKSEQ("\\u5C0D", 0x5C0D);
    CHECKSEQ("\\u0372", 0x0372);
    CHECKSEQ("\\x7F", 0x7F);
    CHECKSEQ("\\x00", 0);
    CHECKSEQ("\\U0010FF85", 0x0010FF85);

    #undef CHECKSEQ

    buf_len = 0;
    EXPECT_FALSE(cat_utf8_or_escape_sequence(p = buf, buf_len, 'a'));

    buf_len = 5;
    EXPECT_FALSE(cat_utf8_or_escape_sequence(p = buf, buf_len, 0x00F1));
    EXPECT_FALSE(cat_utf8_or_escape_sequence(p = buf, buf_len, 0x5C0D));

    buf_len = 9;
    EXPECT_FALSE(cat_utf8_or_escape_sequence(p = buf, buf_len, 0x0010FF85));
}
