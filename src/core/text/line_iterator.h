#ifndef _67534959c9f111e487ba3c15c2ddfeb4
#define _67534959c9f111e487ba3c15c2ddfeb4

#include "core/text/str_view.h"

namespace intent {
namespace core {
namespace text {

struct line_iterator {
    line_iterator() {}
    line_iterator(char const * begin, char const * end=nullptr);
    line_iterator(str_view const & txt);
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
