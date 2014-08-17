#ifndef intent_core_strutil_inline_h
#define intent_core_strutil_inline_h

#include <cstring>

#include "core/strutil.h"

namespace intent {
namespace core {

inline char const * find_char(char const * p, char c, char const * end) {
    if (p == nullptr) return nullptr;
    if (end < p) return p;
    for (; p < end; ++p) {
        if (*p == c) {
            return p;
        }
    }
    return end;
}

inline char const * find_any_char(char const * p, char const * any, char const * end) {
    if (p == nullptr) return nullptr;
    if (end < p) return p;
    for (; p < end; ++p) {
        if (strchr(any, *p)) {
            return p;
        }
    }
    return end;
}

inline char const * ltrim(char const * begin, char const * end, char const * chars) {
    if (begin == nullptr) return nullptr;
    if (end < begin) return begin;
    if (chars == nullptr) chars = ANY_WHITESPACE;
    while (begin < end) {
        if (strchr(chars, *begin) == nullptr) {
            return begin;
        }
        ++begin;
    }
    return begin;
}

inline char const * rtrim(char const * begin, char const * end, char const * chars) {
    if (begin == nullptr) return nullptr;
    if (end < begin) return begin;
    if (chars == nullptr) chars = ANY_WHITESPACE;
    while (--end >= begin) {
        if (strchr(chars, *end) == nullptr) {
            return end + 1;
        }
    }
    return begin;
}

inline bool is_null_or_empty(char const * p) {
    return p == nullptr || *p == 0;
}

inline char const * scan_spaces_and_tabs(char const * p, char const * end) {
    while (p < end) {
        char c = *p;
        if (c != ' ' && c != '\t') {
            break;
        }
        ++p;
    }
    return p;
}

inline char const * consume_line_break(char const * p, char const * end) {
    if (p + 1 < end) {
        if (*p == '\r') {
            if (p[1] == '\n') {
                ++p;
            }
        }
    }
    ++p;
    return p;
}

}} // end namespace

#endif // sentry
