#include <cmath>
#include <string>

#include "core/scan_numbers.h"
#include "core/unicode.h"
#include "lang/lexer.h"

using std::string;

namespace intent {
namespace lang {

char const * scan_line(char const * p, char const * end);
char const * scan_quoted_string(char const * p, char const * end);
char const * scan_spaces(char const * p, char const * end);
bool get_number_token(char const * p, char const * end, token & t);
bool get_quoted_string_token(char const * p, char const * end, token & t);
char const * scan_phrase(char const * p, char const * end);

lexer::lexer(char const * begin) :
    txt(begin), line_begin(begin), p(nullptr), line_number(1) {
    t.substr.end = begin;
    advance();
}

lexer::lexer(char const * begin, char const * end) :
    txt(begin, end), line_begin(begin), p(nullptr), line_number(1) {
    t.substr.end = begin;
    advance();
}

lexer::lexer(sslice const & txt) :
    txt(txt), line_begin(txt.begin), p(nullptr), line_number(1) {
    t.substr.end = txt.begin;
    advance();
}

bool lexer::advance() {

    t.type = tt_invalid;
    p = t.substr.begin = t.substr.end;

    if (t.substr.begin >= txt.end) return false;

    switch (*p) {
    case 0:
        return false;
    case '\r':
        if (p < txt.end - 1 && p[1] == '\n') ++p;
        // fall through; don't break
    case '\n':
        ++line_number;
        t.substr.end = p + 1;
        line_begin = t.substr.end;
        t.type = tt_line_break;
        break;
    case ' ':
        t.substr.end = scan_spaces(p + 1, txt.end);
        if (t.substr.begin == line_begin) {
            if (!indenter) {
                indenter.assign(t.substr.begin, p);
                t.type = tt_indent;
            } else if (t.substr.size() % indenter.size() == 0) {
                t.type = tt_indent;
            }
        }
        break;
    case '#':
        t.substr.end = scan_line(p + 1, txt.end);
        t.type = tt_private_comment;
        break;
    case '|':
        t.substr.end = scan_line(p + 1, txt.end);
        t.type = tt_doc_comment;
        break;
    case '"':
        get_quoted_string_token(p, txt.end, t);
        break;
    case '-':
    case '+':
        if (p < txt.end - 1 && isdigit(p[1])) {
            get_number_token(p, txt.end, t);
        } else {
            t.type = tt_invalid;
            t.substr.end = p + 1;
        }
        break;

    default:
        {
            char c = *p;
            if (isalpha(c)) {
                t.substr.end = scan_phrase(p + 1, txt.end);
                t.type = tt_noun_phrase;
            } else if (isdigit(c)) {
                get_number_token(p, txt.end, t);
            } else {
                t.type = tt_invalid;
                t.substr.end = p + 1;
            }
        }
        break;
    }
    return true;
}

inline char const * scan_line(char const * p, char const * end) {
    while (p < end) {
        char c = *p;
        if (c == '\n' || c == '\r') {
            break;
        }
        ++p;
    }
    return p;
}

char const * scan_quoted_string(char const * first_content_char, char const * end, string & value) {
    char const * p = first_content_char;
    value.clear();
    while (p < end) {
        bool handled = false;
        char c = *p;
        if (c == '"') {
            break;
        } else if (c == '\\') {
            codepoint_t cp;
            // TODO: tweak scan_unicode_escape_sequence() so it honors end.
            char const * seq_end = scan_unicode_escape_sequence(p + 1, cp);
            if (cp != UNICODE_REPLACEMENT_CHAR && seq_end > p && seq_end <= end) {
                handled = true;
                char buf[16];
                size_t buf_length = sizeof(buf);
                char * tmp = buf;
                cat_codepoint_to_utf8(tmp, buf_length, cp);
                // TODO: prove that cat_codepoint_to_utf8 worked.
                value += buf;
                p = seq_end;
            }
        }
        if (!handled) {
            value += c;
            ++p;
        }
    }
    return p;
}

inline bool get_quoted_string_token(char const * p, char const * end, token & t) {
    t.type = tt_quoted_string;
    string value;
    char const * close_quote = scan_quoted_string(p + 1, end, value);
    t.value = value;
    if (close_quote != end) {
        t.substr.end = close_quote + 1;
        return true;
    } else {
        t.substr.end = close_quote;
        return false;
    }
}

inline char const * scan_spaces(char const * p, char const * end) {
    while (p < end) {
        if (*p != ' ') {
            break;
        }
        ++p;
    }
    return p;
}


void set_possibly_signed_value(token & t, bool negative, uint64_t n) {
    if (negative) {
        t.value = -1 * static_cast<int64_t>(n);
    } else {
        t.value = n;
    }
}

/**
 * Read a numeric token at the specified offset, and update t with correct
 * information about what it contains.
 * @return true if a valid number token existed at the offset.
 */
bool get_number_token(char const * p, char const * end, token & t) {

    uint64_t whole_number = 0;

    char c = *p;

    bool negative = (c == '-');
    if (c == '+' || c == '-') {
        c = *++p;
    }

    bool floating_point = (c == '.');

    if (!floating_point) {
        if (c == '0') {
            if (p + 2 < end) {
                c = p[1];
                token_type tt = tt_invalid;
                if (c == 'x' || c == 'X') {
                    t.substr.end = scan_hex_digits(p + 2, end, whole_number);
                    tt = tt_hex_number;
                } else if (c == 'b' || c == 'B') {
                    t.substr.end = scan_binary_digits(p + 2, end, whole_number);
                    tt = tt_binary_number;
                } else if (c >= '0' && c <= '7') {
                    t.substr.end = scan_octal_digits(p + 2, end, whole_number);
                    tt = tt_octal_number;
                }
                if (tt != tt_invalid) {
                    t.type = tt;
                    set_possibly_signed_value(t, negative, whole_number);
                    return true;
                }
            }
        }
        p = scan_decimal_digits_pre_radix(p, end, whole_number);
    }

    double significand, value;

    if (!floating_point && p < end && *p == '.') {
        floating_point = true;
        p = scan_decimal_digits_post_radix(++p, end, significand);
    }

    // Now we've read everything except possibly an exponent. Make sure we've
    // combined values to left and right of radix, if appropriate.
    if (floating_point) {
        significand += whole_number;
    }

    // Check for exponent.
    if (p + 1 < end && (*p == 'e' || *p == 'E')) {
        floating_point = true;

        bool negative_exponent;
        uint64_t exponent;
        p = scan_decimal_number(++p, end, negative_exponent, exponent);
        double exp = exponent;
        if (negative_exponent) {
            exp *= -1;
        }
        // TODO: what if exponent is too big? What if there's nothing after "e"?
        value = significand * pow(10, exp);
        if (negative) {
            value *= -1;
        }
    }

    t.substr.end = p;
    if (floating_point) {
        t.type = tt_floating_point_number;
        t.value = value;
    } else {
        t.type = tt_decimal_number;
        set_possibly_signed_value(t, negative, whole_number);
    }

    return p;
}

char const * scan_phrase(char const * p, char const * end) {
    while (p < end) {
        if (!isalpha(*p)) {
            break;
        }
        ++p;
    }
    return p;
}

} // end namespace lang
} // end namespace intent
