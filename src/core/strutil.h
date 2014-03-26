#ifndef _2ae84901285444e9a2d091accbbd2d7e
#define _2ae84901285444e9a2d091accbbd2d7e

char const * const ANY_WHITESPACE = " \t\r\n";
char const * const LINE_WHITESPACE = " \t";

char const * ltrim(char const * begin, char const * end, char const * chars=ANY_WHITESPACE);
char const * rtrim(char const * begin, char const * end, char const * chars=ANY_WHITESPACE);
bool is_null_or_empty(char const * p);

#include "strutil-inline.h"

#endif // sentry
