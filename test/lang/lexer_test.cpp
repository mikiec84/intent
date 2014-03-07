#include <algorithm>

#include "core/countof.h"
#include "lang/lexer.h"
#include "gtest/gtest.h"

using namespace intent::lang;

TEST(lexer_test, iterator_on_empty_str) {
    lexer lex("");
    token t;
    EXPECT_EQ(0, std::count (lex.begin(), lex.end(), t));
}

TEST(lexer_test, DISABLED_numbers) {
    char const * numbers[] = {
        "0", "-1", "36121", "123_456_789", "0x03", "0b0100100", "0023",
    };
    for (int i = 0; i < countof(numbers); ++i) {
        lexer lex(numbers[i]);
        auto it = lex.begin();
        EXPECT_TRUE(is_number_literal(it->type));
    }

}
