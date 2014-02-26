#ifndef _8cb04c776a874c52b121ffba3ba0348c
#define _8cb04c776a874c52b121ffba3ba0348c

#include <string.h>

inline char const * ltrim(char const * begin, char const * end, char const * chars) {
    while (begin < end) {
        if (strchr(chars, *begin) == nullptr) {
            return begin;
        }
        ++begin;
    }
    return begin;
}

inline char const * rtrim(char const * begin, char const * end, char const * chars) {
    while (end > begin) {
        if (strchr(chars, *end) == nullptr) {
            return end;
        }
        --end;
    }
    return end;
}

#endif
