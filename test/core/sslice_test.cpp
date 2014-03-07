#include "core/sslice.h"

#include "gtest/gtest.h"

TEST(sslice_test, size) {

    sslice t1("hello");
    EXPECT_EQ(5, t1.size());
    EXPECT_FALSE(t1.empty());
    EXPECT_TRUE(t1);

    sslice t2("");
    EXPECT_EQ(0, t2.size());
    EXPECT_TRUE(t2.empty());
    EXPECT_FALSE(t2);

}

