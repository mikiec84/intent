#ifndef _strutil_h_2ae84901285444e9a2d091accbbd2d7e
#define _strutil_h_2ae84901285444e9a2d091accbbd2d7e

#include <string>
#include <vector>

struct sslice;

char const * const ANY_WHITESPACE = " \t\r\n";
char const * const LINE_WHITESPACE = " \t";
char const * const EIGHTY_SPACES =
        "          ""          ""          ""          "
        "          ""          ""          ""          ";

/**
 * Like strchr, but uses end rather than null terminator as end point.
 */
char const * find_char(char const * p, char c, char const * end);

/**
 * Like strpbrk, but uses end rather than null terminator as end point.
 */
char const * find_any_char(char const * p, char const * any, char const * end);

/**
 * Find first char at or after begin that is not trimmable. If all chars are
 * trimmable, return end.
 */
char const * ltrim(char const * begin, char const * end, char const *
    trimmable = ANY_WHITESPACE);

/**
 * Scan string from end to begin; find first char that is trimmable. This
 * represents the point where a null terminator could be inserted to truncate
 * the string with no trailing whitespace. If all chars are trimmable, return
 * begin.
 *
 *   char const * p = "hello   ";          // 8 chars wide; idx 5 has 1st space
 *   char const * end = p + 6;             // points to null terminator
 *   char const * new_end = rtrim(p, end); // will point to p + 5, where space is
 */
char const * rtrim(char const * begin, char const * end, char const *
    trimmable = ANY_WHITESPACE);

bool is_null_or_empty(char const * p);

/**
 * Scan p until we find end of spaces and tabs, or until we hit end. Return
 * first char that's not a space or a tab, or end if we ran out of characters
 * to scan.
 */
char const * scan_spaces_and_tabs(char const * p, char const * end);

/**
 * Consume any form of line break -- \n, \r\n, or even \r by itself.
 * @pre p points either to \r (which may or may not be followed by \n),
 *     or to \n (not preceded by \r).
 * @return pointer to first char after line break
 */
inline char const * consume_line_break(char const * p, char const * end);

/**
 * Split a string into a vector of std::string or of sslice, depending on
 * selected template.
 */
template <typename T>
std::vector<T> split(char const * p, char const * splitters);

template <> std::vector<std::string> split(char const * p, char const * splitters);
template <> std::vector<sslice> split(char const * p, char const * splitters);


#include "strutil-inline.h"

#endif // sentry
