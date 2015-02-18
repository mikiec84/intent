#include "lang/ast/node.h"
#include "gtest/gtest.h"

using namespace intent::lang::ast;

TEST(node_test, null_parent) {
    node n(nullptr);
    EXPECT_EQ(nullptr, n.get_parent());
}
