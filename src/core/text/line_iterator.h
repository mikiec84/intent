#ifndef intent_core_line_iterator_h
#define intent_core_line_iterator_h

#include "core/text/str_view.h"

namespace intent {
namespace core {
namespace text {

struct line_iterator {
    line_iterator() {}
    line_iterator(char const * begin, char const * end=nullptr);
    bool operator ==(line_iterator const & rhs) const { return line == rhs.line; }
    bool operator !=(line_iterator const & rhs) const { return !(*this == rhs); }
    operator bool() const { return line.operator bool(); }
    str_view & operator *() { return line; }
    str_view * operator ->() { return &line; }
    line_iterator & operator++();
private:
    str_view line;
    char const * end;
};

}}} // end namespace

#endif // sentry
