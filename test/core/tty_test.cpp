#include "core/tty.h"

#include "gtest/gtest.h"

using namespace intent::core::tty;

TEST(tty_test, sanity) {
    if (is_a_tty()) {
        EXPECT_TRUE(get_tty_column_count() > 8);
        EXPECT_TRUE(get_tty_column_count() < 240);
        EXPECT_TRUE(get_tty_row_count() > 0);
        EXPECT_TRUE(get_tty_column_count() < 2400);
    } else {
        EXPECT_EQ(0U, get_tty_column_count());
        EXPECT_EQ(0U, get_tty_row_count());
    }
}
