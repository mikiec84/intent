#include "core/base_x_encode.h"

#include "gtest/gtest.h"

using namespace intent::core::text;

#define check10(n) EXPECT_TRUE(base_x_encode(n, buf, sizeof(buf) - 1, "0123456789", 10)); EXPECT_STREQ(#n, buf)
#define check16(n, hex) EXPECT_TRUE(base_x_encode(n, buf, sizeof(buf) - 1, "0123456789abcdef", 16)); EXPECT_STREQ(hex, buf)
#define check2(n, binary) EXPECT_TRUE(base_x_encode(n, buf, sizeof(buf) - 1, "01", 2)); EXPECT_STREQ(binary, buf)

TEST(base_x_encode_test, easy) {
    char buf[64];

    check10(25034);
    check10(0);
    check10(10);
    check10(1000000);

    check16(25034, "61ca");
    check16(0, "0");
    check16(255, "ff");
    check16(256, "100");

    check2(25034, "110000111001010");
}

TEST(base_x_encode_test, bad_params) {
    char buf[64];
    EXPECT_FALSE(base_x_encode(5, nullptr, sizeof(buf) - 1, "0123456789", 10));
    EXPECT_FALSE(base_x_encode(5, buf, 0, "0123456789", 10));
    EXPECT_FALSE(base_x_encode(5, buf, sizeof(buf) - 1, nullptr, 10));
    EXPECT_FALSE(base_x_encode(5, buf, sizeof(buf) - 1, "", 10));
}

TEST(base_x_encode_test, overflow) {
    char buf[64];
    EXPECT_FALSE(base_x_encode(555555, buf, 1, "0123456789", 10));
}

TEST(base_x_encode_test, complex) {
    char buf[64];
    // 4675940 = 21^5 + (3*21^4) + (19*21^2) + 17
    // ... this means we should get a representation like this:
    //     --in the leftmost column (21^5), the "1" digit, which is "b"
    //     --next (21^4), the "3" digit, which is "d"
    //     --next (21^3), the "0" digit, which is "a"
    //     --next (21^2), the "19" digit, which is "t"
    //     --next (21^1), the "0" digit, which is "a"
    //     --last (21^0), the "17" digit, which is "r"
    EXPECT_TRUE(base_x_encode(4675940, buf, sizeof(buf) - 1, "abcdefghijklmnopqrstu", 21));
    EXPECT_STREQ("bdatar", buf);
}
