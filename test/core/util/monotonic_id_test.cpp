#include "core/util/monotonic_id.h"

#include "gtest/gtest.h"

using intent::core::util::monotonic_id;


TEST(monotonic_id_test, simple) {
    monotonic_id<unsigned> simple_id;
    ASSERT_EQ(0u, simple_id.next());
    ASSERT_EQ(1u, simple_id.next());
    ASSERT_EQ(2u, simple_id.next());
    simple_id.set_next(5);
    ASSERT_EQ(5u, simple_id.next());
}


TEST(monotonic_id_test, custom_increment) {
    monotonic_id<int> fancy_id(-23, -5);
    ASSERT_EQ(-23, fancy_id.next());
    ASSERT_EQ(-28, fancy_id.next());
    ASSERT_EQ(-33, fancy_id.next());
    fancy_id.set_next(3);
    ASSERT_EQ(3, fancy_id.next());
    ASSERT_EQ(-2, fancy_id.next());
}
