#ifndef intent_core_wrap_lines_h
#define intent_core_wrap_lines_h

#include <string>

#include "core/text/str_view-fwd.h"
#include "core/text/unicode.h"

namespace intent {
namespace core {
namespace text {

typedef char const * (*wrap_lines_advance_func)(char const *);
char const * next_utf8_char(char const * p);

std::string wrap_lines(str_view const & input, unsigned width=80,
    char const * line_delim="\n", wrap_lines_advance_func nxt=next_utf8_char);

}}} // end namespace

#endif // sentry

