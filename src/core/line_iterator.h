#ifndef _line_iterator_h_ade0a606459645039152b22d3c95e1e2
#define _line_iterator_h_ade0a606459645039152b22d3c95e1e2

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
