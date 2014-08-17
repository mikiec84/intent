#ifndef intent_core_line_iterator_h
#define intent_core_line_iterator_h

#include "core/sslice.h"

namespace intent {
namespace core {

struct line_iterator {
    line_iterator() {}
    line_iterator(char const * begin, char const * end=nullptr);
    bool operator ==(line_iterator const & rhs) const { return line == rhs.line; }
    bool operator !=(line_iterator const & rhs) const { return !(*this == rhs); }
    operator bool() const { return line.operator bool(); }
    sslice & operator *() { return line; }
    sslice * operator ->() { return &line; }
    line_iterator & operator++();
private:
    sslice line;
    char const * end;
};

}} // end namespace

#endif // sentry
