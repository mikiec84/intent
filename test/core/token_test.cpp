#include "core/token.h"

#include "gtest/gtest.h"

TEST(token_test, size) {
    token t("hello");
    ASSERT_EQ(5, t.size());
}

