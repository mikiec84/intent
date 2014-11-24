#include "core/util/scope_guard.h"

#include "gtest/gtest.h"

TEST(scope_guard_test, basic_usage) {
    bool fired = false;
    {
        on_scope_exit(fired = true);
    }
    ASSERT_TRUE(fired);
    fired = false;
    {
        named_scope_exit(x, fired = true);
        x.dismiss();
    }
    ASSERT_FALSE(fired);
}

TEST(scope_guard_test, free) {
    char * memory = (char *)malloc(50);
    bool freed = false;
    {
        on_scope_exit(freed = true; free(memory));
    }
    ASSERT_TRUE(freed);
}

TEST(scope_guard_test, named_scope_exits_have_different_names) {
    int i = 3;
    named_scope_exit(a, i = 2);
    named_scope_exit(b, i = 1);
    // If the above compiles, then named scope guards don't have colliding symbols.
}

TEST(scope_guard_test, unnamed_scope_exits_have_different_names) {
    int i = 3;
    on_scope_exit(i = 2);
    on_scope_exit(i = 1);
    // If the above compiles, then named scope guards don't have colliding symbols.
}

