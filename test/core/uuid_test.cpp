#include "core/uuid.h"

#include "gtest/gtest.h"

// This test is not attempting to exercise any particular functionality; we
// assume the boost libraries have adequate testing already, and that they work
// as advertised. Instead, we just prove that we can use a boost library within
// its intent namespace.
TEST(uuid_test, available) {
    intent::core::uuids::uuid x;
    ASSERT_TRUE(x.is_nil());
}
