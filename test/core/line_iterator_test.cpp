#include "core/line_iterator.h"
#include "gtest/gtest.h"

using namespace intent::core;

TEST(line_iterator_test, default_is_end) {
    line_iterator it;
    ASSERT_FALSE(it);
}

TEST(line_iterator_test, non_empty_is_not_end) {
    line_iterator it("test");
    ASSERT_TRUE(it);
}

TEST(line_iterator_test, empty_is_not_end) {
    line_iterator it("");
    ASSERT_FALSE(it);
    ASSERT_FALSE(*it == null_sslice);
}

void check_lines(char const * txt, std::initializer_list<char const *> lines) {
    line_iterator it(txt);
    for (unsigned i = 0; ; ++i) {
        if (i == lines.size()) {
            break;
        }
        char const * this_line = lines.begin()[i];
        if (*it == null_sslice) {
            ADD_FAILURE() << "Expected line " << i + 1 << " to be returned, but iterator ended.";
            return;
        } else {
            if (strcmp(*it, this_line) != 0) {
                ADD_FAILURE() << "Expected line " << i + 1 << " to equal \""
                              << this_line << "\", not \"" << *it << "\"";
            }
            ++it;
        }
    }
    if (it) {
        ADD_FAILURE() << "Iterator returned more than " << lines.size() << " lines.";
    }
}

TEST(line_iterator_test, various_lines) {
    check_lines("this\nis\r\na\rtest\n\n", {"this", "is", "a", "test", "", ""});
}
