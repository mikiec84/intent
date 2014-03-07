#ifndef _0adec40c2b864af2ab1651428d80a55c
#define _0adec40c2b864af2ab1651428d80a55c

#include <cstring>

#include "core/sslice.h"

inline sslice::sslice() :
    begin(nullptr), end(nullptr) {
}

inline sslice::sslice(char const * begin, char const * end) {
    assign(begin, end);
}

inline sslice::sslice(char const * begin) {
    assign(begin);
}

inline size_t sslice::size() const {
    return end - begin;
}

inline bool sslice::empty() const {
    return end == begin;
}

inline sslice::operator bool() const {
    return end > begin;
}

inline bool sslice::operator ==(sslice const & rhs) const {
    return begin == rhs.begin && end == rhs.end;
}

inline bool sslice::operator !=(sslice const & rhs) const {
    return !(*this == rhs);
}

inline sslice & sslice::assign(char const * _begin, char const * _end) {
    begin = _begin;
    end = (_begin && (_end > _begin)) ? _end : _begin;
    return *this;
}

inline sslice & sslice::assign(char const * _begin) {
    begin = _begin;
    end = _begin ? strchr(_begin, 0) : nullptr;
    return *this;
}


#endif // sentry
