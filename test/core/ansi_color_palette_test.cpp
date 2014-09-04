#include "core/ansi_color_palette.h"

#include "gtest/gtest.h"

using namespace intent::core::tty;

TEST(ansi_color_palette_test, easy_ctor) {
    ansi_color_palette p(
        "status=3\n"
        "warning=4\n"
        "error=5"
    );
    EXPECT_EQ(3U, p.map_name("status"));
    EXPECT_EQ(4U, p.map_name("warning"));
    EXPECT_EQ(5U, p.map_name("error"));
}

TEST(ansi_color_palette_test, more_complex) {
    ansi_color_palette p(
        "header=14\n"
        "see-also=2\n"
        "callout=10\n"
        "error=9\n"
        "cross-ref=15\n"
        "warning=11\n"
        "keyword=6\n"
        "optional=4\n"
        "normal=8\n"
        "note=3\n"
        "hidden=0\n"
    );
    EXPECT_EQ(15U, p.map_name("cross"));
    EXPECT_EQ(10U, p.map_name("call"));
    EXPECT_EQ(9U, p.map_name("error"));
}
