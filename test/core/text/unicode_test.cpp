#include "core/text/unicode.h"

#include "gtest/gtest.h"

using namespace intent::core::text;


TEST(unicode_test, is_utf8_lead_byte) {
	// Disallowed because they would be used for overlong encodings.
	EXPECT_FALSE(is_utf8_lead_byte(static_cast<char>(0xc0)));
	EXPECT_FALSE(is_utf8_lead_byte(static_cast<char>(0xc1)));

	EXPECT_TRUE(is_utf8_lead_byte(static_cast<char>(0xc2)));
	EXPECT_TRUE(is_utf8_lead_byte(static_cast<char>(0xd8)));
	EXPECT_FALSE(is_utf8_lead_byte(static_cast<char>(0x80)));
	EXPECT_FALSE(is_utf8_lead_byte(static_cast<char>(0x85)));
	EXPECT_FALSE(is_utf8_lead_byte(static_cast<char>(0xa8)));
	EXPECT_FALSE(is_utf8_lead_byte('x'));
	EXPECT_FALSE(is_utf8_lead_byte(static_cast<char>(0)));
}


TEST(unicode_test, is_surrogate) {
	EXPECT_TRUE(is_surrogate(0xD800));
	EXPECT_TRUE(is_surrogate(0xDFFF));
	EXPECT_TRUE(is_surrogate(0xDBFF));
	EXPECT_TRUE(is_surrogate(0xDC00));
	EXPECT_TRUE(is_surrogate(0xD900));
	EXPECT_TRUE(is_surrogate(0xDE00));
	EXPECT_FALSE(is_surrogate(0x1DE00));
	EXPECT_FALSE(is_surrogate(0x10DE00));
	EXPECT_FALSE(is_surrogate(0xFF00DE00));
}


TEST(unicode_test, is_high_surrogate) {
	EXPECT_TRUE(is_high_surrogate(0xD800));
	EXPECT_FALSE(is_high_surrogate(0xDFFF));
	EXPECT_TRUE(is_high_surrogate(0xDBFF));
	EXPECT_FALSE(is_high_surrogate(0xDC00));
	EXPECT_TRUE(is_high_surrogate(0xD900));
	EXPECT_FALSE(is_high_surrogate(0xDE00));
	EXPECT_FALSE(is_high_surrogate(0x1D900));
	EXPECT_FALSE(is_high_surrogate(0x10D900));
	EXPECT_FALSE(is_high_surrogate(0xFF00D900));
}


TEST(unicode_test, is_low_surrogate) {
	EXPECT_FALSE(is_low_surrogate(0xD800));
	EXPECT_TRUE(is_low_surrogate(0xDFFF));
	EXPECT_FALSE(is_low_surrogate(0xDBFF));
	EXPECT_TRUE(is_low_surrogate(0xDC00));
	EXPECT_FALSE(is_low_surrogate(0xD900));
	EXPECT_TRUE(is_low_surrogate(0xDE00));
	EXPECT_FALSE(is_low_surrogate(0x1DE00));
	EXPECT_FALSE(is_low_surrogate(0x10DE00));
	EXPECT_FALSE(is_low_surrogate(0xFF00DE00));
}


TEST(unicode_test, is_utf8_continuation_byte) {
	EXPECT_FALSE(is_utf8_continuation_byte(static_cast<char>(0xc0)));
	EXPECT_FALSE(is_utf8_continuation_byte(static_cast<char>(0xd8)));
	EXPECT_FALSE(is_utf8_continuation_byte(static_cast<char>(0xf5)));
	EXPECT_TRUE(is_utf8_continuation_byte(static_cast<char>(0x80)));
	EXPECT_TRUE(is_utf8_continuation_byte(static_cast<char>(0xa8)));
	EXPECT_FALSE(is_utf8_continuation_byte('x'));
	EXPECT_FALSE(is_utf8_continuation_byte(static_cast<char>(0)));
}


TEST(unicode_test, predict_length_of_codepoint_from_lead_byte) {
	EXPECT_EQ(1u, predict_length_of_codepoint_from_lead_byte('x'));
	EXPECT_EQ(2u, predict_length_of_codepoint_from_lead_byte('\xC2'));
	EXPECT_EQ(3u, predict_length_of_codepoint_from_lead_byte('\xE2'));
	EXPECT_EQ(4u, predict_length_of_codepoint_from_lead_byte('\xF2'));
	EXPECT_EQ(0u, predict_length_of_codepoint_from_lead_byte(0xC0));
	EXPECT_EQ(0u, predict_length_of_codepoint_from_lead_byte(0xC1));
	EXPECT_EQ(0u, predict_length_of_codepoint_from_lead_byte(0xF5));
	EXPECT_EQ(0u, predict_length_of_codepoint_from_lead_byte(0xFF));
	EXPECT_EQ(0u, predict_length_of_codepoint_from_lead_byte(0x85));
}


TEST(unicode_test, is_well_formed_utf8) {
	EXPECT_FALSE(is_well_formed_utf8(nullptr));
	EXPECT_TRUE(is_well_formed_utf8(""));
	EXPECT_TRUE(is_well_formed_utf8("\r\n"));

	// Mixture of normal and double-byte sequences as used in latin langs.
	EXPECT_TRUE(is_well_formed_utf8("\xC2\xA9 2001, Chang\xC3\xA9 Corp."));

	// Japanese, Russian, Greek 3-byte sequences -- with a little ASCII
	EXPECT_TRUE(is_well_formed_utf8("\xE5\xA4\x89\xE3\x82\x8F\xD1\x81\xD0\xB2 ascii \xD1\x8F\xD0\xB7\xCF\x8E\xCF\x81\xCE\xB1"));

	// overlong encoding of the Euro sign
	EXPECT_FALSE(is_well_formed_utf8("\xF0\x82\x82\xAC"));

	// overlong encoding of embedded null
	EXPECT_FALSE(is_well_formed_utf8("with embedded null \xC0\x80 <<there"));

	// CESU-8-style surrogate pair (should be disallowed; see http://j.mp/1HzJPBY)
	EXPECT_FALSE(is_well_formed_utf8("\xED\xA0\x81\xED\xB0\x80"));

	// invalid trail bytes, per table 3.7 in the Unicode Standard v7, Section
	// Conformance 3.9, Table 3-7, Well-Formed UTF-8 Byte Sequences.
	// http://www.unicode.org/versions/Unicode7.0.0/ch03.pdf#G7404
	EXPECT_FALSE(is_well_formed_utf8("\xE0\x9F\xB1")); // bad second byte
	EXPECT_FALSE(is_well_formed_utf8("\xED\xA0\xB1")); // bad second byte
	EXPECT_FALSE(is_well_formed_utf8("\xF0\x85\xB1\xB1")); // bad second byte
	EXPECT_FALSE(is_well_formed_utf8("\xF4\x90\xB1\xB1")); // bad second byte
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
