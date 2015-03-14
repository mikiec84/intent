#ifndef _98c1669e872d416dba695eee9bcda4bd
#define _98c1669e872d416dba695eee9bcda4bd

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
