#include "core/pstr.h"

#include "gtest/gtest.h"

TEST(pstr_test, size) {
    pstr s("hi");
    ASSERT_EQ(2, s.size());
}
