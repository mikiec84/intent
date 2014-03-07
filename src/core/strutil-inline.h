#ifndef _8cb04c776a874c52b121ffba3ba0348c
#define _8cb04c776a874c52b121ffba3ba0348c

#include <cstring>

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
    while (--end > begin) {
        if (strchr(chars, *end) == nullptr) {
            return end + 1;
        }
    }
    return begin;
}

#endif
