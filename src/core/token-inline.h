#ifndef _0adec40c2b864af2ab1651428d80a55c
#define _0adec40c2b864af2ab1651428d80a55c

#include <string.h>

#include "core/token.h"

inline token::token(char const * txt, char const * end) : txt(txt), end(end) {
}

inline token::token(char const * txt) : txt(txt), end(strchr(txt, 0)) {
}

inline size_t token::size() const {
    return end - txt;
}

#endif // sentry
