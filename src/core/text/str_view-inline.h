#ifndef _820049b2e6ac411f8e2c13d23d9718c5
#define _820049b2e6ac411f8e2c13d23d9718c5

#include <algorithm>
#include <cstring>

#include "core/text/str_view.h"

namespace intent {
namespace core {
namespace text {


template <typename C>
inline strview<C>::strview() :
    begin(nullptr), length(0) {
}


template <typename C>
inline strview<C>::strview(C const * _begin, C const * _end) {
    assign(_begin, _end);
}


template <typename C>
inline strview<C>::strview(C const * _begin, size_t _length) :
    begin(_begin), length(_length) {
}


template <typename C>
inline strview<C>::strview(C const * _begin) {
    assign(_begin);
}


template <typename C>
inline strview<C>::strview(std::basic_string<C> const & str) {
    assign(str.c_str(), str.size());
}


template <typename C>
inline strview<C>::strview(std::basic_string<C> const & str, size_t len) {
    assign(str.c_str(), std::min(length, str.size()));
}


template <typename C>
inline void strview<C>::make_null() {
    begin = nullptr;
    length = 0;
}


template <typename C>
inline void strview<C>::begin_at(const C * new_begin) {
    if (new_begin > begin) {
        auto shortened = static_cast<size_t>(new_begin - begin);
        length = (shortened >= length) ? 0: length - shortened;
        begin = new_begin;
    } else if (new_begin < begin) {
        if (new_begin) {
            length += (begin - new_begin);
        } else {
            length = 0;
        }
        begin = new_begin;
    }
}


template <typename C>
inline void strview<C>::end_at(const C * new_end) {
    if (begin) {
        length = (new_end >= begin) ? new_end - begin : 0;
    }
}


template <typename C>
inline C const * strview<C>::end() const {
    return begin + length;
}


template <typename C>
inline bool strview<C>::is_empty() const {
    return length == 0;
}


template <typename C>
inline bool strview<C>::is_null() const {
    return begin == nullptr;
}


template <typename C>
inline strview<C>::operator bool() const {
    return !is_empty();
}


template <typename C>
inline bool strview<C>::operator ==(strview<C> const & rhs) const {
    return begin == rhs.begin && length == rhs.length;
}


template <typename C>
inline bool strview<C>::operator !=(strview<C> const & rhs) const {
    return !(*this == rhs);
}


template <typename C>
inline strview<C> & strview<C>::assign(C const * _begin, C const * end) {
    begin = _begin;
    length = (_begin && (end > _begin)) ? end - begin : 0;
    return *this;
}


template <typename C>
inline strview<C> & strview<C>::assign(C const * _begin, size_t _length) {
    begin = _begin;
    length = _length;
    return *this;
}


template <typename C>
inline strview<C> & strview<C>::assign(C const * _begin) {
    begin = _begin;
    length = _begin ? strlen(_begin) : 0;
    return *this;
}


template <typename C>
inline int strcmp(C const * a, strview<C> const & b) {
    if (a) {
        if (!b.is_null()) {
            int n = ::strncmp(a, b.begin, b.length);
            if (n == 0) {
                n = (a[b.length] ? 1 : 0);
            }
            return n;
        } else {
            return 1;
        }
    } else {
        return (!b.is_null()) ? -1 : 0;
    }
}


template <typename C>
inline int strcmp(strview<C> const & a, C const * b) {
    if (!a.is_null()) {
        if (b) {
            int n = ::strncmp(a.begin, b, a.length);
            if (n == 0) {
                n = (b[a.length] ? -1 : 0);
            }
            return n;
        } else {
            return 1;
        }
    } else {
        return b ? -1 : 0;
    }
}


template <typename C>
inline int strcmp(strview<C> const & a, strview<C> const & b) {
    if (!a.is_null()) {
        if (!b.is_null()) {
            int n = ::strncmp(a.begin, b.begin, std::min(a.length, b.length));
            if (n == 0) {
                n = static_cast<int>(a.length) - static_cast<int>(b.length);
            }
            return n;
        } else {
            return 1;
        }
    } else {
        return b ? -1 : 0;
    }
}


template <typename C>
inline int strncmp(C const * a, strview<C> const & b, size_t length) {
    if (a) {
        if (!b.is_null()) {
            return ::strncmp(a, b.begin, std::min(length, b.length));
        } else {
            return 1;
        }
    } else {
        return (b.length) ? -1 : 0;
    }
}


template <typename C>
inline int strncmp(strview<C> const & a, C const * b, size_t length) {
    if (!a.is_null()) {
        if (b) {
            return ::strncmp(a.begin, b, std::min(length, a.length));
        } else {
            return 1;
        }
    } else {
        return b ? -1 : 0;
    }
}


template <typename C>
inline int strncmp(strview<C> const & a, strview<C> const & b, size_t length) {
    if (!a.is_null()) {
        if (!b.is_null()) {
            return ::strncmp(a.begin, b.begin, std::min(length, std::min(a.length, b.length)));
        } else {
            return 1;
        }
    } else {
        return b ? -1 : 0;
    }
}


}}} // end namespace

#endif // sentry
