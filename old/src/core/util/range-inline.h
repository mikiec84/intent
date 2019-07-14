#ifndef _60488c7576114509a8d114bc777e73a9
#define _60488c7576114509a8d114bc777e73a9

namespace intent {
namespace core {
namespace util {

template <typename IDX>
inline range<IDX>::range() : begin(static_cast<IDX>(0)), end(begin) {
}

template <typename IDX>
inline range<IDX>::range(index_t b, index_t e) : begin(b), end(e) {
}

template <typename IDX>
inline range<IDX>::operator bool() const {
    return end > begin;
}

template <typename IDX>
inline bool range<IDX>::operator ==(range const & rhs) const {
    return begin == rhs.begin && end == rhs.end;
}

template <typename IDX>
inline bool range<IDX>::operator <(range const & rhs) const {
    if (begin < rhs.begin) return true;
    if (begin > rhs.begin) return false;
    return end < rhs.end;
}

template <typename IDX>
inline size_t range<IDX>::length() const {
    return end > begin ? static_cast<size_t>(end - begin) : 0;
}

template <typename IDX>
inline range<IDX> & range<IDX>::operator =(range<IDX> const & rhs) {
    begin = rhs.begin;
    end = rhs.end;
    return *this;
}

template <typename IDX>
inline range<IDX> & range<IDX>::operator =(range<IDX> && rhs) {
    begin = rhs.begin;
    end = rhs.end;
    return *this;
}

}}} // end namespace

#include "core/text/str_view-inline.h"

#endif
