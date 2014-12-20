#include <boost/filesystem.hpp>

#include "core/text/arg.h"
#include "core/util/countof.h"
#include "core/text/str_view.h"

#include "gtest/gtest.h"

using namespace intent::core::text;

const char * const txt = "abcxyz";
str_view const slc(txt + 1, txt + 4); // bcx
boost::filesystem::path file("/root/subdir/x.txt");
std::string str_arg("my string");

arg test_args[] = {
    25,
    static_cast<int32_t>(1943872801),
    static_cast<uint64_t>(2598239487120349ULL),
    3.14,
    "howdy",
    str_arg,
    true,
    false,
    slc,
    file
};

char const * test_arg_strs[] = {
    "25",
    "1943872801",
    "2598239487120349",
    "3.14",
    "howdy",
    "my string",
    "true",
    "false",
    "bcx",
    "/root/subdir/x.txt"
};

TEST(arg_test, snprintf) {
    char buf[64];
    for (unsigned i = 0; i < countof(test_args); ++i) {
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
    for (unsigned i = 0; i < countof(test_args); ++i) {
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
    for (unsigned i = 0; i < countof(test_args); ++i) {
        unsigned n = test_args[i].snprintf(buf, 0);
        if (n != strlen(test_arg_strs[i])) {
            // From http://linux.die.net/man/3/snprintf:
            // The functions snprintf() and vsnprintf() do not write more than
            // size bytes (including the terminating null byte ('\0')). If the
            // output was truncated due to this limit then the return value is
            // the number of characters (excluding the terminating null byte)
            // which would have been written to the final string if enough space
            // had been available. Thus, a return value of size or more means
            // that the output was truncated.
            ADD_FAILURE() << "For item " << i << ", expected \""
                          << strlen(test_arg_strs[i]) << "\", but got \""
                          << n << "\".";
        }
    }
}
