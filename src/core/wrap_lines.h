#ifndef intent_core_wrap_lines_h
#define intent_core_wrap_lines_h

#include <string>

#include "unicode.h"

namespace intent {
namespace core {

struct sslice;

typedef char const * (*wrap_lines_advance_func)(char const *);
char const * next_utf8_char(char const * p);

std::string wrap_lines(sslice const & input, unsigned width=80,
    char const * line_delim="\n", wrap_lines_advance_func nxt=next_utf8_char);

}} // end namespace

#endif // sentry

