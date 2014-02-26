#include "core/strutil.h"

#include "gtest/gtest.h"

typedef char const * (*trim_func)(char const *, char const *, char const *);

void try_trim(trim_func trimmer, char const * cstr, char const * expected, char const * chars=ANY_WHITESPACE) {
    char const * end = strchr(cstr, 0);
    EXPECT_STREQ(expected, trimmer(cstr, end, chars));
}

TEST(strutil_test, ltrim) {
    try_trim(ltrim, " \ttest", "test");
    try_trim(ltrim, "test ", "test ");
}

