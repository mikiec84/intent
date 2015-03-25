#ifndef _b6aa28646e434b5da7c32f234c4dedf8
#define _b6aa28646e434b5da7c32f234c4dedf8

#include <initializer_list>

#include "core/text/arg.h"

namespace intent {
namespace core {
namespace text {

/**
 * Interpolate args into a format string. This function is conceptually similar to
 * the sprintf() family, except that it is type-safe, does not require you to supply
 * a buffer, and supports an extended argument syntax.
 *
 * References to args are enclosed by curly braces, and are numbered from 1: {1}, {2},
 * {3}, and so forth. References may appear in any order, and may be repeated. Besides
 * their identifying number, references may contain other rich meta information in
 * additional sections before the closing curly brace. For legibility, whitespace may
 * occur between sections, although it may not occur between the open curly brace and
 * the identifying number of the reference.
 *
 * References to args may have an optional sprintf-style format specifier section after
 * their identifying number: {1 %.2f}, {2 %04x}, and so forth.
 *
 * References may also have an optional comment to clarify their meaning. If present,
 * the comment is delimited by an = and is always at the end of the arg reference,
 * just before the closing curly brace: {3 %4.1f =albedo of planet}, {1 =name}, etc.
 * Such comments can be very helpful in localization.
 *
 * A special convention facilitates the common task of pluralizing certain words or
 * otherwise tweaking a string based on the value of an argument (typically, a count).
 * An example might look like this:
 *
 *     interp("found {1} error{1 ~ s||s}", error_count);
 *
 * Here, we are passing only one argument. It is referenced twice. The first time, the
 * reference is normal. The second time, the special tweaking syntax is invoked by the
 * tilde after the number. The tilde is followed by a set of two or more string
 * possibilities, delimited by the pipe (vertical bar) character. These possiblities
 * are selected based on the numeric value of whichever arg is referenced. The first
 * candidate is selected if the arg has a value of 0, the second if it has a value of
 * 1, the third if it has a value of 2, the fourth if it has a value of 3, and so on.
 * The practical effect is that the string "found 1 error" will be printed if arg 1
 * has the numeric value of 1, and "found 0 errors" or "found 2 errors" will be
 * printed if arg 1 has a value of 0 or 2, respectively.
 *
 * The numeric value of an arg is its integer value if the arg is numeric. For booleans,
 * false = 0 and true = 1. For strings, empty/null = 0, and other values = 1. The last
 * candidate is assumed to apply to all values bigger than the previous one, or to
 * negative values.
 *
 * The motivation for this feature is to eliminate the antipattern where a conditional
 * is used to change a format string, like this:
 *
 *     sprintf(buf, "found %d error%s", error_count, error_count == 1 ? "" : "s");
 *
 * This sort of usage is completely incompatible with localization; a translator has no
 * opportunity to translate the suffix, can't alter the criterion whereby a suffix is
 * selected, and may not pluralize with a suffix at all. By putting the inflection into
 * the string, the translator has full visibility and control. It also makes the code
 * cleaner.
 */
std::string interp(char const * format, std::initializer_list<arg>);

/**
 * Just for consistency, we allow interp with no args.
 */
std::string interp(char const * format);

/**
 * Similar to interp(), except that data is appended to an existing string. This allows
 * repeated calls to build a complex string in stages, and it lets other functions
 * implement interp-like features.
 */
void interp_into(std::string &, char const * format, std::initializer_list<arg>);

}}} // end namespace

#endif // sentry
