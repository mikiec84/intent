#include <algorithm>

#include "lang/lexer.h"

#include "gtest/gtest.h"

using namespace intent::lang;

TEST(lexer_test, iterator_on_empty_str) {
    lexer lex("");
    token t;
    EXPECT_EQ(0, std::count (lex.begin(), lex.end(), t));
}
