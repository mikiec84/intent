#include "core/pstr.h"

#include "gtest/gtest.h"

using namespace intent::core;

TEST(pstr_test, size) {
    pstr s("hi");
    ASSERT_EQ(2U, s.size());
}
