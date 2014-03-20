#include <algorithm>

#include "core/countof.h"
#include "lang/lexer.h"
#include "gtest/gtest.h"

using std::string;
using boost::any_cast;

using namespace intent::lang;

TEST(lexer_test, indent_and_dedent) {
    lexer lex(
                "1\n"
                "\t11\n"
                "\t12\n"
                "\t\t121\n"
                "\t\t122\n"
                "20\n"
                "30\n"
                "\t31\n"
                );
    token_type expected[] = {
        tt_decimal_number,
        tt_indent,
        tt_decimal_number,
        tt_decimal_number,
        tt_indent,
        tt_decimal_number,
        tt_decimal_number,
        tt_dedent,
        tt_dedent,
        tt_decimal_number,
        tt_decimal_number,
        tt_indent,
        tt_decimal_number,
        tt_dedent
    };
    auto it = lex.begin();
    for (int i = 0; i < countof(expected); ++i) {
        if (expected[i] != it->type) {
            FAIL() << "Token[" << i << "] was " << get_token_type_name(it->type)
                   << " instead of " << get_token_type_name(expected[i]) << ".";
        }
        ++it;
        if (it == lex.end() && i < countof(expected) - 1) {
            FAIL() << "Ended after Token[" << i << "].";
        }
    }
}

TEST(lexer_test, DISABLED_quoted_string) {
    struct test_item {
        char const * txt;
        char const * expected_value;
    };

    #define QUOTE(str) "\"" str "\""
    test_item const quoted_strs[] = {
        {QUOTE("\\x7F \\u5C0D "), "\x7F \xE5\xB0\x8D "},
        {QUOTE("abc"), "abc"},
        {QUOTE(""), ""},
        {QUOTE("\x7F \xE5\xB0\x8D "), "\x7F \xE5\xB0\x8D "},
        {QUOTE("\\r\\n\\t\\b\\v\\a\\f \\u "), "\r\n\t\b\v\a\f \\u "},
        {QUOTE("abc\n... def"), "abc def"},
    };
    for (int i = 0; i < countof(quoted_strs); ++i) {
        auto txt = quoted_strs[i].txt;
        lexer lex(txt);
        auto it = lex.begin();
        if (!is_string_literal(it->type)) {
            ADD_FAILURE() << "Expected \"" << txt << "\" to be parsed as a quoted string; got "
                          << get_token_type_name(it->type) << " instead.";
        }
        string const & v = any_cast<string const &>(it->value);
        EXPECT_STREQ(quoted_strs[i].expected_value, v.c_str());
        if (++it != lex.end()) {
            ADD_FAILURE() << "Did not consume full quoted string with \"" << txt << "\".";
        }
    }
}

TEST(lexer_test, iterator_on_empty_str) {
    lexer lex("");
    token t;
    EXPECT_EQ(0, std::count (lex.begin(), lex.end(), t));
}

TEST(lexer_test, numbers) {
    char const * number_strs[] = {
        "0", "-1", "+25.7", "36121", "123_456_789", "0x03", "0b0100100", "0023", "3.7e-5",
    };

    for (int i = 0; i < countof(number_strs); ++i) {
        lexer lex(number_strs[i]);
        auto it = lex.begin();
        if (!is_number_literal(it->type)) {
            ADD_FAILURE() << "Expected \"" << number_strs[i] << "\" to be parsed as a numeric token; got "
                          << get_token_type_name(it->type) << " instead.";
        }
        if (++it != lex.end()) {
            ADD_FAILURE() << "Did not consume full numeric token with \"" << number_strs[i] << "\".";
        }
    }
}

TEST(lexer_test, DISABLED_malformed_numbers) {
#if 0
    What if a number has multiple underscores in a row, is all underscores, or has
    underscores in a trailing position?

    What if we have overflow or underflow?

    What about width suffixes on numbers?

    What about signed hex/binary/octal?

    What about long doubles and arbitrary precision?

    What about complex numbers?
#endif
}
