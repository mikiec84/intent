#include "sslice.h"
#include "strutil.h"

namespace {

template <typename T>
std::vector<T> split_impl(char const * txt, char const * splitters) {
    typedef std::vector<T> returned_t;
    returned_t returned;
    if (txt && *txt) {
        char const * begin = nullptr;
        char const * p = txt;
        for (; *p; ++p) {
            if (strchr(splitters, *p)) {
                if (begin) {
                    returned.push_back(T(begin, p));
                    begin = nullptr;
                }
            } else {
                if (!begin) {
                    begin = p;
                }
            }
        }
        if (begin) {
            returned.push_back(T(begin, p));
        }
    }
    return std::move(returned);
}

} // end anonymous namespace

template <>
std::vector<std::string> split(char const * p, char const * splitters) {
    return split_impl<std::string>(p, splitters);
}

template <>
std::vector<sslice> split(char const * p, char const * splitters) {
    return split_impl<sslice>(p, splitters);
}
