#include "lang/code_site.h"
#include "gtest/gtest.h"

using namespace intent::lang;

TEST(code_site_test, ctor_with_no_copy_of_url) {
    dependent_code_site cs("x", 1, 2);
    EXPECT_STREQ("x", cs.compilation_unit_url);
    EXPECT_EQ(1U, cs.line_number);
    EXPECT_EQ(2U, cs.offset_on_line);
}

TEST(code_site_test, copy_url) {
    char buf[8];
    strcpy(buf, "x");
    {
        independent_code_site cs(buf, 1, 2);
    }
    // If we can do this, then we haven't incorrectly deleted buf;
    // instead, we've deleted our private copy of buf.
    strcpy(buf, "hello");
}
