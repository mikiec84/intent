#include "core/arg.h"
#include "core/countof.h"

#include "gtest/gtest.h"

std::string str_arg("my string");
arg test_args[] = {
    25,
    1943872801L,
    2598239487120349ULL,
    3.14,
    "howdy",
    str_arg,
    true,
    false
};

char const * test_arg_strs[] = {
    "25",
    "1943872801",
    "2598239487120349",
    "3.14",
    "howdy",
    "my string",
    "true",
    "false"
};

TEST(arg_test, snprintf) {
    char buf[64];
    for (int i = 0; i < countof(test_args); ++i) {
        buf[0] = 0;
        test_args[i].snprintf(buf, sizeof(buf));
        if (strcmp(test_arg_strs[i], buf) != 0) {
            ADD_FAILURE() << "For item " << i << ", expected \""
                          << test_arg_strs[i] << "\", but got \""
                          << buf << "\".";
        }
    }
}

TEST(arg_test, to_string) {
    for (int i = 0; i < countof(test_args); ++i) {
        auto s = test_args[i].to_string();
        if (strcmp(test_arg_strs[i], s.c_str()) != 0) {
            ADD_FAILURE() << "For item " << i << ", expected \""
                          << test_arg_strs[i] << "\", but got \""
                          << s << "\".";
        }
    }
}

TEST(arg_test, snprintf_overflow) {
    char buf[64];
    buf[0] = 1;
    buf[1] = 0;
    for (int i = 0; i < countof(test_args); ++i) {
        auto n = test_args[i].snprintf(buf, 0);
        if (n != strlen(test_arg_strs[i])) {
            ADD_FAILURE() << "For item " << i << ", expected \""
                          << strlen(test_arg_strs[i]) << "\", but got \""
                          << n << "\".";
        }
    }
}
