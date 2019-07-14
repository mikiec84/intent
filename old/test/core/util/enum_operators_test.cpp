#include "core/util/enum_operators.h"

#include "gtest/gtest.h"

enum class pickle : unsigned {
    dill,
    bread_and_butter,
};

enum class tee_shirt_size : short {
    extra_small,
    small,
    medium,
    large,
    xlarge,
    xxlarge
};
define_numeric_operators_for_enum(tee_shirt_size);

enum traversal_strategies {
    kbreadth_first = 1,
    kdepth_first = 2,
    krandom = 4,
};
define_bitwise_operators_for_enum(traversal_strategies);

TEST(enum_operators_test, numeric_operators) {
    auto size = tee_shirt_size::small;
    size += 1;
    ASSERT_EQ(tee_shirt_size::medium, size);
    ASSERT_EQ(tee_shirt_size::large, size + 1);
    ASSERT_EQ(tee_shirt_size::small, size - 1);
    ASSERT_EQ(tee_shirt_size::large, ++size);
    ASSERT_EQ(tee_shirt_size::large, size);
    ASSERT_EQ(tee_shirt_size::large, size++);
    ASSERT_EQ(tee_shirt_size::xlarge, size);
    ASSERT_EQ(tee_shirt_size::xlarge, size--);
    ASSERT_EQ(tee_shirt_size::large, size);
    ASSERT_EQ(tee_shirt_size::medium, --size);
    ASSERT_EQ(tee_shirt_size::medium, size);
    size -= 1;
    ASSERT_EQ(tee_shirt_size::small, size);
}

TEST(enum_operators_test, wont_compile_because_numeric_operators_disabled) {
#if 0 // change to 1 to verify that you get compile error
    pickle p = pickle::dill;
    p = p - 1;
#endif
}

TEST(enum_operators_test, bitwise_operators) {
    ASSERT_EQ(3, kbreadth_first | kdepth_first);
    ASSERT_EQ(kbreadth_first, kbreadth_first & 3);
    ASSERT_EQ(kbreadth_first | krandom, ~(kbreadth_first ^ ~krandom));
    auto n = kbreadth_first;
    n |= kdepth_first;
    ASSERT_EQ(3, n);
    n &= kdepth_first;
    ASSERT_EQ(kdepth_first, n);
}

TEST(enum_operators_test, wont_compile_because_bitwise_operators_disabled) {
#if 0 // change to 1 to verify that you get compile error
    pickle p = pickle::dill;
    p = p & pickle::bread_and_butter;
#endif
}
