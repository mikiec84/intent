#include "lang/ast/node.h"
#include "gtest/gtest.h"

using namespace intent::lang::ast;

TEST(node_test, null_parent) {
    node n(nullptr);
    EXPECT_EQ(nullptr, n.get_parent());
    node m(&n);
    EXPECT_EQ(&n, m.get_parent());
}


TEST(node_test, root) {
    node n(nullptr);
    node m(&n);
    node o(&m);
    EXPECT_EQ(&n, o.get_root());
    EXPECT_EQ(&n, n.get_root());
    EXPECT_EQ(&n, m.get_root());
}


TEST(node_test, children) {
    node n(nullptr);
    EXPECT_EQ(0u, n.get_child_count());
    n.add_child(nullptr);
    EXPECT_EQ(0u, n.get_child_count());
    auto a = new node(&n);
    auto b = new node(&n);
    n.add_child(a);
    n.add_child(b);
    EXPECT_EQ(2u, n.get_child_count());
    EXPECT_EQ(a, n.get_child(0));
    EXPECT_EQ(b, n.get_child(1));
    for (unsigned i = 0; i < 200; ++i) {
        n.add_child(new node(&n));
    }
    EXPECT_EQ(202u, n.get_child_count());
}
