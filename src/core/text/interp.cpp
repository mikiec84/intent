#include <cstring>
#include "core/text/interp.h"
#include "core/text/scan_numbers.h"
#include "core/text/strutil.h"

using std::initializer_list;
using std::string;

namespace intent {
namespace core {
namespace text {

string interp(char const * format) {
    return format;
}

char const * scan_arg(char const * p, char const * end, string & txt,
                      initializer_list<arg> args, unsigned max_arg) {

    const size_t format_specifier_buflen = 32;

    uint64_t n;
    char const * end_of_section = scan_decimal_digits_pre_radix(p, end, n);
    if (n > 0 && static_cast<unsigned>(n) <= max_arg) {

        arg const & a = *(args.begin() + n-1);
        // At this point, it's very likely (but still not guaranteed) that we have a
        // valid arg ref. We should see a } in very short order, before any line breaks.
        char const * end_of_arg = find_any_char(end_of_section, "}\r\n", end);
        if (end_of_arg < end && *end_of_arg == '}') {
            char const * next = scan_spaces_and_tabs(end_of_section, end);
            switch (*next) {
            case '%':
                end_of_section = find_any_char(next, "}=", end);
                end_of_section = rtrim(next, end_of_section);
                if (end_of_section < next + format_specifier_buflen) {
                    char format_specifier[format_specifier_buflen];
                    strncpy(format_specifier, next, end_of_section - next);
                    format_specifier[end_of_section - next] = 0;
                    txt += a.to_string(format_specifier);
                    return end_of_arg;
                }
                break;
            case '}':
            case '=':
                txt += a.to_string();
                return end_of_arg;
            case '~':
                {
                    int value_to_compare;
                    switch (a.type) {
                    case arg::vt_empty:
                        value_to_compare = 0; break;
                    case arg::vt_signed:
                        value_to_compare = static_cast<int>(a.i64); break;
                    case arg::vt_unsigned:
                        value_to_compare = static_cast<int>(a.u64); break;
                    case arg::vt_float:
                        value_to_compare = static_cast<int>(a.dbl); break;
                    case arg::vt_date:
                        value_to_compare = 0; break;
                    case arg::vt_str:
                        value_to_compare = a.str->empty() ? 0 : 1; break;
                    case arg::vt_cstr:
                        value_to_compare = a.cstr && a.cstr[0] ? 1 : 0; break;
                    case arg::vt_bool:
                        value_to_compare = a.boolean ? 1 : 0; break;
                    default:
                        value_to_compare = 0; break;
                    }

                    char const * begin = ltrim(next + 1, end_of_arg);
                    for (int j = 0; ; ++j) {
                        end_of_section = find_char(begin, '|', end_of_arg);
                        if ((j == value_to_compare && end_of_section < end) || end_of_section == end_of_arg) {
                            end_of_section = rtrim(begin, end_of_section);
                            if (end_of_section > begin) {
                                txt += string(begin, end_of_section);
                            }
                            return end_of_arg;
                        }
                        begin = end_of_section + 1;
                    }
                }
                break;
            }
        }
    }
    // Failed to see valid arg def; return p unmodified.
    return p;
}

void interp_into(string & txt, char const * format, initializer_list<arg> args) {

    char const * end = strchr(format, 0);
    unsigned max_arg = static_cast<unsigned>(args.size());
    if (max_arg > 99) {
        max_arg = 99;
    }

    // Reserve a chunk of memory all at once, instead of piecemeal.
    // We won't get this exactly right, but we'll probably get close,
    // without wasting a lot of memory.
    txt.reserve(txt.size() + (end - format) + (max_arg * 6));
    for (char const * p = format; p != end; ++p) {
        char c = *p;

        // Minimum size of an arg ref is 3 chars: {n}. See if we have a curly brace,
        // and if a whole ref will fit.
        if (c == '{' && p + 2 < end) {
            if (p[1] == '{') {
                ++p;
            } else {
                // Do we have a valid arg? If yes, consume any chars it included, and
                // iterate again immediately. Otherwise, write out raw characters.
                ++p;
                auto end_of_arg = scan_arg(p, end, txt, args, max_arg);
                if (end_of_arg > p) {
                    p = end_of_arg;
                    continue;
                } else {
                    txt += '{';
                    c = *p;
                }
            }
        }
        txt += c;
    }
}

std::string interp(char const * format, std::initializer_list<arg> args) {
    std::string txt;
    interp_into(txt, format, args);
    return txt;
}

}}} // end namespace
