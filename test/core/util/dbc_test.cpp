#define contract_violation_action(contract_type, expr) \
    report_violation(contract_type, expr, __FILE__, __LINE__, __FUNCTION__)

#include "core/util/dbc.h"

#include "gtest/gtest.h"

using namespace intent::core::util;

bool should_throw = true;
unsigned violation_count = 0;

static void report_violation(contract_type type, char const * expr,
    char const * file, int line, char const * function) {
    ++violation_count;
    if (should_throw) {
        contract_violation x(type, expr, file, line, function);
        throw x;
    }
}

TEST(dbc_test, custom_action) {
    should_throw = false;
    on_scope_exit(should_throw = true);
    auto vcount = violation_count;
    // This check should call our custom action, but NOT trigger a
    // throw because should_throw == false.
    intracondition(0 == 1);
    ASSERT_EQ(vcount + 1, violation_count);
}

static inline char const * get_fname(char const * path) {
    auto p = strrchr(path, '/');
    auto q = strrchr(path, '\\');
    if (q > p) {
        p = q;
    }
    return p ? (p + 1) : path;
}

TEST(dbc_test, precondition) {
    constexpr char * txt = "hi";
    auto line_num = 0;
    try {
        line_num = __LINE__; precondition(txt == nullptr);
        FAIL() << "Expected precondition to throw an exception.";
    } catch (contract_violation const & cv) {
        ASSERT_EQ(contract_type::pre, cv.ctype);
        ASSERT_TRUE(strstr(cv.what(), "txt == nullptr"));
        auto file = get_fname(__FILE__);
        ASSERT_TRUE(strstr(cv.what(), file));
        char buf[20];
        sprintf(buf, "%d", line_num);
        ASSERT_TRUE(strstr(cv.what(), buf));
    }
    try {
        precondition("hi" != nullptr);
    } catch (...) {
        FAIL() << "Did not expect precondition to throw an exception.";
    }
}

TEST(dbc_test, check) {
    try {
        intracondition(3 < 2);
        FAIL() << "Expected intracondition to throw an exception.";
    } catch (contract_violation const & cv) {
        ASSERT_EQ(contract_type::check, cv.ctype);
    }
    try {
        intracondition(3 > 2);
    } catch (...) {
        FAIL() << "Did not expect intracondition to throw an exception.";
    }
}

TEST(dbc_test, contract_violation_is_catchable) {
    bool caught = false;
    try {
        throw contract_violation(contract_type::post, "i == 0", "x.cpp", 25, "func");
    } catch (contract_violation const & cv) {
        caught = true;
    }
    ASSERT_TRUE(caught);
}

TEST(dbc_test, postcondition) {
    should_throw = false;
    on_scope_exit(should_throw = true);

    auto vcount = violation_count;
    int i = 0;
    {
        postcondition(i > 3); // should only be evaluated at end of scope, not now
        i = 5;
    }
    if (violation_count > vcount) {
        FAIL() << "Did not expect postcondition to trip, since the value of i"
                  " was 5 when we went out of scope.";
    }
    {
        i = 0;
        postcondition(i == 3);
        i = 5;
    }
    if (vcount == violation_count) {
        FAIL() << "Expected postcondition to trip, since the value of i was 5"
                  " when we went out of scope.";
    }
}

TEST(dbc_test, failed_precondition_aborts_program_if_not_overridden) {
    ASSERT_DEATH({
        int i = 0;
        postcondition(i == 1);
    }, "Failed postcondition");
}
