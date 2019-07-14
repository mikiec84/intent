#ifndef _e1f6b48631ad4fd8a5999e9376bdbe8c
#define _e1f6b48631ad4fd8a5999e9376bdbe8c

#include <string>
#include <vector>

#include "core/text/str_view-fwd.h"
#include "core/util/range.h"

namespace intent {
namespace core {
namespace text {

typedef core::util::range<uint32_t> str_range;

constexpr char const * const ANY_WHITESPACE = " \t\r\n";
constexpr char const * const LINE_WHITESPACE = " \t";
constexpr char const * const EIGHTY_SPACES =
        "          ""          ""          ""          "
        "          ""          ""          ""          ";

/**
 * Like strchr, but uses end rather than null terminator as end point.
 */
char const * find_char(char const * p, char c, char const * end);

/**
 * Like strchr, but uses a range rather than null terminator.
 */
char const * find_char(char const * p, char c, str_range range);

/**
 * Like strpbrk, but uses end rather than null terminator as end point.
 */
char const * find_any_char(char const * p, char const * any, char const * end);

/**
 * Like strpbrk, but uses range rather than null terminator.
 */
char const * find_any_char(char const * p, char const * any, str_range range);

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
char const * consume_line_break(char const * p, char const * end);

/**
 * Split a string into a vector of std::string or of str_view, depending on
 * selected template.
 */
template <typename T>
std::vector<T> split(char const * p, char const * splitters);

template <> std::vector<std::string> split(char const * p, char const * splitters);
template <> std::vector<str_view> split(char const * p, char const * splitters);


/** Similar to tolower(), but explicitly limited to ascii and implemented as inline.*/
char ascii_to_lower_case(char c);

/**
 * Similar to strlwr(); converts bytes in the range A-Z to bytes in the range
 * a-z. Not locale- or codepage-aware; however, any codepage that maps 7-bit
 * values the same way as ASCII can be safely passed to this function; no 8-bit
 * bytes will be altered.
 */
char * ascii_to_lower_case(char * p);
char * ascii_to_lower_case(char * p, char const * end);


/** Similar to toupper(), but explicitly limited to ascii and implemented as inline.*/
char ascii_to_upper_case(char c);


/**
 * Similar to strlwr(); converts bytes in the range a-z to bytes in the range
 * A-Z. Not locale- or codepage-aware; however, any codepage that maps 7-bit
 * values the same way as ASCII can be safely passed to this function; no 8-bit
 * bytes will be altered.
 */
char * ascii_to_upper_case(char * p);
char * ascii_to_upper_case(char * p, char const * end);


/**
 * Similar to strcasecmp() or stricmp(); compares strings in a case-insensitive
 * way, using only ASCII assumptions that A-Z and a-z are equivalent. Not locale-
 * or codepage-aware.
 */
int compare_str_ascii_case_insensitive(char const * a, char const * b);


/**
 * Like islower(), but inlined, and explicit about only supporting ascii.
 */
bool is_ascii_lower(char c);


/**
 * Like isupper(), but inlined, and explicit about only supporting ascii.
 */
bool is_ascii_upper(char c);


/**
 * Like isalpha(), but inlined, and explicit about only supporting ascii.
 */
bool is_ascii_alpha(char c);


/**
 * Like isalnum(), but inlined, and explicit about only supporting ascii.
 */
bool is_ascii_alphanumeric(char c);


/**
 * Like isdigit(), but inlined, and explicit about only supporting ascii.
 */
bool is_ascii_digit(char c);


/**
 * Like isxdigit(), but inlined, and explicit about only supporting ascii.
 * Not case-sensitive.
 */
bool is_ascii_hex(char c);


/**
 * Like isspace(), but inlined, and explicit about only supporting ascii.
 */
bool is_ascii_whitespace(char c);


/**
 * Like isspace(), but inlined and doesn't treat vertical tab (char 11) or
 * formfeed (char 12) as whitespace.
 */
bool is_ascii_space_tab_or_eol(char c);


/**
 * Returns decimal digit value 0-9, or -1 if char isn't a digit.
 */
int get_ascii_digit_value(char c);


/**
 * Returns decimal digit value 0-15, or -1 if char isn't a hex digit. Not
 * case-sensitive.
 */
int get_ascii_hex_value(char c);


}}} // end namespace

#include "core/text/strutil-inline.h"

#endif // sentry
