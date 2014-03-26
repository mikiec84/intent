#include "lang/note.h"
#include "gtest/gtest.h"

using namespace intent::lang;

TEST(note_test, to_string) {
    note n(ii_unterminated_string_literal, "sample.i", 1, 25);
    auto txt = n.to_string();
    EXPECT_TRUE(strstr(txt.c_str(), "line 1"));
    EXPECT_TRUE(strstr(txt.c_str(), "offset 25"));
    EXPECT_TRUE(strstr(txt.c_str(), "sample.i"));
    EXPECT_TRUE(strstr(txt.c_str(), "String literal ends without a closing quote."));
}
