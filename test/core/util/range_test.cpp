#include <algorithm>
#include <sstream>
#include <string>

#include "core/util/countof.h"
#include "core/util/range.h"

#include "gtest/gtest.h"


using std::stringstream;
using std::string;

typedef intent::core::util::range<int> range_t;


TEST(range_test, default_ctor) {
    range_t r;
    ASSERT_FALSE(r);
    ASSERT_EQ(0, r.begin);
    ASSERT_EQ(0, r.end);
}


TEST(range_test, simple) {
    range_t r(1,2);
    ASSERT_TRUE(r);
    ASSERT_EQ(1u, r.length());
}

void _verify(bool expected, bool actual, range_t const & item, range_t const & compared, int i, int j, char const * expr, stringstream & errors) {
    if (actual != expected) {
        errors << std::boolalpha
            << "With item = " << i << ", [" << item.begin
            << ".." << item.end << "), and compared = " << j
            << ", [" << compared.begin << ".." << compared.end
            << "), \"" << expr << "\" was " << actual
            << ", not " << expected << ".\n";
    }
}

TEST(range_test, comparison_operators) {
    range_t x;
    range_t a(1, 2);
    range_t aa(1, 30);
    range_t b(2, 2);
    range_t bb(2, 10);
    range_t c(a);

    range_t ranges[] = {c, aa, bb, b, a, x};

    // Should be stable, so c should come before a
    std::sort(&ranges[0], &ranges[countof(ranges)]);
    ASSERT_TRUE(ranges[0] == x);
    ASSERT_TRUE(ranges[1] == c);
    ASSERT_TRUE(ranges[2] == a);
    ASSERT_TRUE(ranges[3] == aa);
    ASSERT_TRUE(ranges[4] == b);
    ASSERT_TRUE(ranges[5] == bb);

    stringstream errors;
    int end = countof(ranges);
    for (int i = 0; i < end; ++i) {
        auto & item = ranges[i];
        for (int j = 0; j < end; ++j) {
            auto & compared = ranges[j];

            #define VERIFY(expected, expr) _verify(expected, expr, item, compared, i, j, #expr, errors)

            if (j == i) {
                // Comparison operators on identity...
                VERIFY(true, item == compared);
                VERIFY(true, (compared == item));
                VERIFY(false, (item != compared));
                VERIFY(false, (compared != item));
                VERIFY(false, (compared < item));
                VERIFY(true, (compared <= item));
                VERIFY(false, (compared > item));
                VERIFY(true, (compared >= item));
                VERIFY(false, (item < compared));
                VERIFY(true, (item <= compared));
                VERIFY(false, (item > compared));
                VERIFY(true, (item >= compared));
            } else {
                bool should_be_equal = ((i == 1 && j == 2) || (i == 2 && j == 1));
                VERIFY(should_be_equal, item == compared);
                VERIFY(should_be_equal, compared == item);
                VERIFY(!should_be_equal, item != compared);
                VERIFY(!should_be_equal, compared != item);
                VERIFY(j < i && !should_be_equal, compared < item);
                VERIFY(j <= i || should_be_equal, compared <= item);
                VERIFY(j > i && !should_be_equal, compared > item);
                VERIFY(j >= i || should_be_equal, compared >= item);
                VERIFY(i < j && !should_be_equal, item < compared);
                VERIFY(i <= j || should_be_equal, item <= compared);
                VERIFY(i > j && !should_be_equal, item > compared);
                VERIFY(i >= j || should_be_equal, item >= compared);
            }
        }
    }
    string etxt = errors.str();
    if (!etxt.empty()) {
        FAIL() << etxt;
    }
}
