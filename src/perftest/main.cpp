#include <cstdio>
#include <exception>

#include "perftest/perftest.h"

int main(int argc, char ** argv) {

    auto & all_experiments = timed_experiment::all();
    for (auto ex: all_experiments) {
        ex->run();
    }
    return 0;
}


#include "core/text/strutil.h"

using namespace intent::core::text;

constexpr char const * const short_str = "hello, world";
constexpr char const * const short_str_end = short_str + sizeof(short_str);

constexpr char const * const long_str =
        "The quick brown fox, seeing that he was all alone and feeling somewhat panicked, jumped over the lazy red dog and scampered away.";
constexpr char const * const long_str_end = long_str + sizeof(long_str);

time_this(strutil, strstr_easy_match_at_front_of_str, {
    strstr(short_str, "he");
})

time_this(strutil, strstr_not_found_in_short_str, {
    strstr(short_str, "pickle");
})

time_this(strutil, strstr_not_found_in_long_str, {
    strstr(long_str, "pickle");
})

time_this(strutil, strpbrk_found_in_short_str, {
    strpbrk(short_str, ",xo");
})

time_this(strutil, strpbrk_found_in_long_str, {
    strpbrk(long_str, "zWd");
})

time_this(strutil, find_any_char_found_in_short_str, {
    find_any_char(short_str, "x,o", short_str_end);
})

time_this(strutil, find_any_char_found_in_long_str, {
    find_any_char(long_str, "zWd", long_str_end);
})


