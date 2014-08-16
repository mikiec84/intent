#ifndef _sslice_inline_h_0adec40c2b864af2ab1651428d80a55c
#define _sslice_inline_h_0adec40c2b864af2ab1651428d80a55c

#include <algorithm>
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

inline sslice::sslice(std::string const & str) {
    assign(str.c_str(), str.c_str() + str.size());
}

inline sslice::sslice(std::string const & str, size_t len) {
    assign(str.c_str(), str.c_str() + std::min(len, str.size()));
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

inline int strcmp(char const * a, sslice const & b) {
    if (a) {
        if (b != null_sslice) {
            int n = strncmp(a, b.begin, b.size());
            if (n == 0) {
                n = (a[b.size()] ? 1 : 0);
            }
            return n;
        } else {
            return 1;
        }
    } else {
        return (b != null_sslice) ? -1 : 0;
    }
}

inline int strcmp(sslice const & a, char const * b) {
    if (a != null_sslice) {
        if (b) {
            int n = strncmp(a.begin, b, a.size());
            if (n == 0) {
                n = (b[a.size()] ? -1 : 0);
            }
            return n;
        } else {
            return 1;
        }
    } else {
        return b ? -1 : 0;
    }
}

inline int strcmp(sslice const & a, sslice const & b) {
    if (a != null_sslice) {
        if (b != null_sslice) {
            int n = strncmp(a.begin, b.begin, std::min(a.size(), b.size()));
            if (n == 0) {
                n = static_cast<int>(a.size()) - static_cast<int>(b.size());
            }
            return n;
        } else {
            return 1;
        }
    } else {
        return b ? -1 : 0;
    }
}

inline int strncmp(char const * a, sslice const & b, size_t size) {
    if (a) {
        if (b != null_sslice) {
            return strncmp(a, b.begin, std::min(size, b.size()));
        } else {
            return 1;
        }
    } else {
        return (b != null_sslice) ? -1 : 0;
    }
}

inline int strncmp(sslice const & a, char const * b, size_t size) {
    if (a != null_sslice) {
        if (b) {
            return strncmp(a.begin, b, std::min(size, a.size()));
        } else {
            return 1;
        }
    } else {
        return b ? -1 : 0;
    }
}

inline int strncmp(sslice const & a, sslice const & b, size_t size) {
    if (a != null_sslice) {
        if (b != null_sslice) {
            return strncmp(a.begin, b.begin, std::min(size, std::min(a.size(), b.size())));
        } else {
            return 1;
        }
    } else {
        return b ? -1 : 0;
    }
}

#endif // sentry
