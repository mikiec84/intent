#include "lang/ast/node.h"
#include "gtest/gtest.h"

using namespace intent::lang::ast;

TEST(node_test, null_parent) {
	node n;
	EXPECT_EQ(nullptr, n.get_parent());
	node m(&n);
	EXPECT_EQ(&n, m.get_parent());
}


TEST(node_test, root) {
	node n;
	node m(&n);
	node o(&m);
	EXPECT_EQ(&n, o.get_root());
	EXPECT_EQ(&n, n.get_root());
	EXPECT_EQ(&n, m.get_root());
}


TEST(node_test, relationships) {
	node adam;
	EXPECT_EQ(0u, adam.has_ancestor(nullptr));
	EXPECT_EQ(0u, adam.has_ancestor(&adam));
	EXPECT_EQ(0u, adam.has_descendant(nullptr));
	EXPECT_EQ(0u, adam.has_ancestor(&adam));

	node * descendants[5];
	node * x = &adam;
	for (int i = 0; i < 5; ++i) {
		node * newest = new node();
		descendants[i] = newest;
		x->add_child(newest);
		x = newest;
	}
	EXPECT_EQ(1u, descendants[3]->has_descendant(descendants[4]));
	EXPECT_EQ(1u, descendants[3]->has_ancestor(descendants[2]));
	EXPECT_EQ(2u, descendants[3]->has_ancestor(descendants[1]));
	EXPECT_EQ(3u, descendants[3]->has_ancestor(descendants[0]));
	EXPECT_EQ(4u, descendants[3]->has_ancestor(&adam));
	EXPECT_EQ(5u, adam.has_descendant(descendants[4]));
	EXPECT_EQ(0u, adam.has_ancestor(descendants[4]));
}


TEST(node_test, DISABLED_children) {
	node n;
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
