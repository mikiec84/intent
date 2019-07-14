#include "core/text/str_view.h"

#include "gtest/gtest.h"

using namespace intent::core::text;

TEST(str_view_test, size) {

    str_view t1("hello");
    EXPECT_EQ(5U, t1.length);
    EXPECT_FALSE(t1.is_empty());
    EXPECT_TRUE(t1);

    str_view t2("");
    EXPECT_EQ(0U, t2.length);
    EXPECT_TRUE(t2.is_empty());
    EXPECT_FALSE(t2);

}

