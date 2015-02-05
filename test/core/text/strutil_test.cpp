#include "core/util/countof.h"
#include "core/text/str_view.h"
#include "core/text/strutil.h"

#include "gtest/gtest.h"

using namespace intent::core::text;
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

	auto z = split<str_view>(TXT, ", ");
	EXPECT_EQ(n, z.size());
	for (unsigned i = 0; i < n; ++i) {
		EXPECT_EQ(0, strcmp(y[i], z[i]));
	}
}

TEST(strutil_test, split_with_null_or_empty_input) {
	EXPECT_EQ(0U, split<string>("", "aeiou").size());
	EXPECT_EQ(0U, split<str_view>(nullptr, "aeiou").size());
}


TEST(strutil_test, ascii_to_lower_case) {
	char buf[20];
	strcpy(buf, "HELLO\xC0");
	EXPECT_STREQ("HELLO\xC0", ascii_to_lower_case(buf, &buf[0] - 1));
	EXPECT_STREQ("heLLO\xC0", ascii_to_lower_case(buf, &buf[0] + 2));
	EXPECT_STREQ("hello\xC0", ascii_to_lower_case(buf));
	buf[0] = 0;
	EXPECT_STREQ("", ascii_to_lower_case(buf));
	EXPECT_STREQ(nullptr, ascii_to_lower_case(nullptr));
}


TEST(strutil_test, ascii_to_upper_case) {
	char buf[20];
	strcpy(buf, "hello\xC0");
	EXPECT_STREQ("hello\xC0", ascii_to_lower_case(buf, &buf[0] - 1));
	EXPECT_STREQ("HEllo\xC0", ascii_to_upper_case(buf, &buf[0] + 2));
	EXPECT_STREQ("HELLO\xC0", ascii_to_upper_case(buf));
	buf[0] = 0;
	EXPECT_STREQ("", ascii_to_upper_case(buf));
	EXPECT_STREQ(nullptr, ascii_to_upper_case(nullptr));
}


TEST(strutil_test, compare_str_ascii_case_insensitive) {

	EXPECT_EQ(0, compare_str_ascii_case_insensitive(nullptr, nullptr));
	EXPECT_EQ(0, compare_str_ascii_case_insensitive("", ""));

	EXPECT_EQ(1, compare_str_ascii_case_insensitive("hello", nullptr));
	EXPECT_EQ(1, compare_str_ascii_case_insensitive("hello", ""));
	EXPECT_EQ(-1, compare_str_ascii_case_insensitive(nullptr, "hello"));
	EXPECT_EQ(-1, compare_str_ascii_case_insensitive("", "hello"));

	EXPECT_EQ(-1, compare_str_ascii_case_insensitive("hello", "Hi!"));
	EXPECT_EQ(1, compare_str_ascii_case_insensitive("Hi!", "hello"));
}
