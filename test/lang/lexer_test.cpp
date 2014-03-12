#include <algorithm>

#include "core/countof.h"
#include "lang/lexer.h"
#include "gtest/gtest.h"

using namespace intent::lang;

TEST(lexer_test, DISABLED_quoted_string) {
    char const * quoted_strs[] = {
        "\"abc\"", "\"\"", "\"abc\n... def\"",
    };
    for (int i = 0; i < countof(quoted_strs); ++i) {
        lexer lex(quoted_strs[i]);
        auto it = lex.begin();
        if (!is_string_literal(it->type)) {
            ADD_FAILURE() << "Expected \"" << quoted_strs[i] << "\" to be parsed as a quoted string; got "
                          << get_token_type_name(it->type) << " instead.";
        }
        if (++it != lex.end()) {
            ADD_FAILURE() << "Did not consume full quoted string with \"" << quoted_strs[i] << "\".";
        }
    }
}

TEST(lexer_test, iterator_on_empty_str) {
    lexer lex("");
    token t;
    EXPECT_EQ(0, std::count (lex.begin(), lex.end(), t));
}

TEST(lexer_test, numbers) {
    // TODO: rewrite so we not only verify that the resulting token is a number, but also
    // that it has the correct value.
    char const * numbers[] = {
        "0", "-1", "+25.7", "36121", "123_456_789", "0x03", "0b0100100", "0023", "3.7e-5",
    };
    for (int i = 0; i < countof(numbers); ++i) {
        lexer lex(numbers[i]);
        auto it = lex.begin();
        if (!is_number_literal(it->type)) {
            ADD_FAILURE() << "Expected \"" << numbers[i] << "\" to be parsed as a numeric token; got "
                          << get_token_type_name(it->type) << " instead.";
        }
        if (++it != lex.end()) {
            ADD_FAILURE() << "Did not consume full numeric token with \"" << numbers[i] << "\".";
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
