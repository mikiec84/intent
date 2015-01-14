#include "core/util/advanced_macros.h"
//#include "core/util/stringize.h"

#include "gtest/gtest.h"

TEST(advanced_macros_test, stringize) {
    ASSERT_STREQ("advanced_macros_test", stringize(advanced_macros_test));

    #define A_NUMERIC_VALUE 17
    ASSERT_STREQ("17", stringize(17));
    ASSERT_STREQ("A_NUMERIC_VALUE", stringize(A_NUMERIC_VALUE));
    ASSERT_STREQ("17", stringize_value_of(A_NUMERIC_VALUE));
}

TEST(advanced_macros_test, glued_tokens) {
    int glue_tokens(a, b);
    // This line will only compile if we glued correctly; otherwise, ab will
    // be undefined.
    ab = 1;

    // Just declaring these variables would be fine, but the compiler complains
    // about unused variables if I don't do something else with them...
    int x = ab ? ab + 1 : 0;
    int glue_token_values(x, __LINE__) = x ? 2 : 0;
    // This line will only compile if we glued correctly; otherwise, it will
    // be a duplicate decl.
    int glue_token_values(x, __LINE__) = 3;
    if (x24 == 0 || x27 == 0) {
        FAIL() << "didn't expect to get here";
    }
}

TEST(advanced_macros_test, count_varargs) {
    ASSERT_EQ(0, count_varargs());
    ASSERT_EQ(1, count_varargs(1));
    ASSERT_EQ(2, count_varargs(1, 2));
    ASSERT_EQ(3, count_varargs(1, 2, 3));
    ASSERT_EQ(4, count_varargs(1, 2, 3, 4));
    ASSERT_EQ(5, count_varargs(1, 2, 3, 4, 5));
    ASSERT_EQ(6, count_varargs(1, 2, 3, 4, 5, 6));
    ASSERT_EQ(7, count_varargs(1, 2, 3, 4, 5, 6, 7));

    ASSERT_EQ(35, count_varargs(
          1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
         21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35));

    ASSERT_EQ(49, count_varargs(
          1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
         21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
         41, 42, 43, 44, 45, 46, 47, 48, 49));

    ASSERT_EQ(50, count_varargs(
          1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
         21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
         41, 42, 43, 44, 45, 46, 47, 48, 49, 50));
}

#define _MY_CONCAT3(a, b, c) a b c
#define _MY_CONCAT2(a, b) a b

#define _GET_MACRO(_1, _2, _3, NAME, ...) NAME
#define MY_CONCAT(...) _GET_MACRO(__VA_ARGS__, _MY_CONCAT3, _MY_CONCAT2)(__VA_ARGS__)

TEST(advanced_macros_test, overloaded_macros) {
    ASSERT_STREQ("abc", MY_CONCAT("a", "b", "c"));
    ASSERT_STREQ("ab", MY_CONCAT("a", "b"));
}

#define QUALIFIER(X) X::
#define OPEN_NS(X)   namespace X {
#define CLOSE_NS(X)  }

call_macro_x_for_each(OPEN_NS,Outer,Next,Inner)
   typedef int MyFoo;
   MyFoo foo() { return 0; }
call_macro_x_for_each(CLOSE_NS,Outer,Next,Inner)

// If this test compiles, it is basically working. What we're exercising here
// is the ability of call_macro_x_for_each to dispatch to the correct override.
TEST(advanced_macros_test, call_macro_x_for_each) {
    #define QUALIFY(NAME,...) call_macro_x_for_each(QUALIFIER,##__VA_ARGS__)NAME

    QUALIFY(MyFoo,Outer,Next,Inner) n =
            call_macro_x_for_each(QUALIFIER, Outer, Next, Inner) foo();
    ASSERT_EQ(0, n);

    QUALIFY(int) x = 1;
    // Avoid "unused variable" warning.
    if (x - n == 0) return;
}

// If this test compiles, it is basically working, just like above. This is
// just alternate syntax.
TEST(advanced_macros_test, with_each) {
    with (each (Outer,Next,Inner), QUALIFIER) MyFoo n =
            call_macro_x_for_each(QUALIFIER, Outer, Next, Inner) foo();
    if (n == 0) return;
}
