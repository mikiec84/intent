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
char const * scan_spaces_and_tabs(char const * p, char const * end);
bool get_number_token(char const * p, char const * end, token & t);
bool get_quoted_string_token(char const * p, char const * end, token & t);
char const * scan_phrase(char const * p, char const * end);

const uint32_t NO_INDENT_YET = UINT32_MAX;
const uint32_t EMPTY_INDENT_STACK = 0;

lexer::lexer(char const * begin) :
    txt(begin), line_begin(begin), p(nullptr), inconsistent_indent(nullptr),
    line_number(1), total_indent_width(0), indent_dedent_delta(0),
    last_stack_insert_idx(NO_INDENT_YET) {

    memset(indent_stack, 0, sizeof(indent_stack));
    t.substr.end = begin;
    advance();
}

lexer::lexer(char const * begin, char const * end) :
    txt(begin, end), line_begin(begin), p(nullptr), inconsistent_indent(nullptr),
    line_number(1), total_indent_width(0), indent_dedent_delta(0),
    last_stack_insert_idx(NO_INDENT_YET) {

    memset(indent_stack, 0, sizeof(indent_stack));
    t.substr.end = begin;
    advance();
}

lexer::lexer(sslice const & txt) :
    txt(txt), line_begin(txt.begin), p(nullptr), inconsistent_indent(nullptr),
    line_number(1), total_indent_width(0), indent_dedent_delta(0),
    last_stack_insert_idx(NO_INDENT_YET) {

    memset(indent_stack, 0, sizeof(indent_stack));
    t.substr.end = txt.begin;
    advance();
}

inline uint32_t get_indent_width(char const * p, char const * end, char indent_char, char const * & inconsistent) {
    uint32_t value = 0;
    inconsistent = nullptr;
    for (; p < end; ++p) {
        char c = *p;
        value += (c == '\t') ? 4 : 1;
        if (inconsistent == nullptr && c != indent_char) {
            inconsistent = p;
        }
    }
    return value;
}

void lexer::push_indent(uint32_t new_indent_width) {
    indent_stack[++last_stack_insert_idx] = new_indent_width;
    total_indent_width += new_indent_width;
    indent_dedent_delta = 1;
}

// Call this each time we find a line break in the code. This lets us do housekeeping
// to indent and dedent, to keep track of which line number we're on, and to suppress
// whitespace tokens that are irrelevant.
char const * lexer::scan_indents_and_lines() {
    while (p < txt.end) {
        ++line_number;
        line_begin = p;
        char const * end_of_indent = scan_spaces_and_tabs(p, txt.end);
        if (end_of_indent == txt.end) {
            return txt.end;
        }
        p = end_of_indent;
        char c = *p;
        if (c == '\r') {
            if (p + 1 < txt.end && p[1] == '\n') {
                ++p;
            }
            c = '\n';
        }
        if (c == '\n') {
            ++p;
        } else {
            // Found a line that's not empty. Look at its indent.
            uint32_t indent_width = get_indent_width(line_begin, end_of_indent,
                                                get_indent_char(), inconsistent_indent);
            if (indent_width > total_indent_width) {
                if (last_stack_insert_idx == NO_INDENT_YET) {
                    indent_stack[0] = *p;
                    last_stack_insert_idx = EMPTY_INDENT_STACK;
                }
                push_indent(indent_width - total_indent_width);
            } else {
                indent_dedent_delta = -1 * static_cast<int>(indent_width);
            }
            break;
        }
    }
    return p;
}

uint32_t lexer::pop_indent() {
    unsigned width = 0;
    if (last_stack_insert_idx > EMPTY_INDENT_STACK) {
        width = indent_stack[last_stack_insert_idx--];
        total_indent_width -= width;
    }
    return width;
}

bool lexer::advance() {

    // Reset state so we report invalid, zero-width token unless/until
    // we report otherwise.
    t.type = tt_invalid;
    t.value = 0;
    p = t.substr.begin = t.substr.end;

    // Before we scan more text, pop any errors or indents/dedents that are queued up.
    if (indent_dedent_delta) {
        if (indent_dedent_delta > 0) {
            indent_dedent_delta = 0;
            t.type = tt_indent;
            return true;
        } else {
            uint32_t popped_width = pop_indent();
            if (total_indent_width <= popped_width) {
                indent_dedent_delta = 0;
                if (total_indent_width < popped_width) {
                    // TODO: issue error
                }
            }
            t.type = tt_dedent;
            return true;
        }
    }

    // If we've exhausted text, report that we can't advance anymore.
    if (t.substr.begin >= txt.end) return false;

    switch (*p) {
    case 0:
        return false;
    case '\r':
        if (p < txt.end - 1 && p[1] == '\n') ++p;
        // fall through; don't break
    case '\n':
        if (++p >= txt.end) return false;
        // Find something besides new lines to look at.
        scan_indents_and_lines();
        // What we need to do now is react to new knowledge we may have
        // about indents and dedents--or to react to some character besides
        // a line break. There are ways to do this by enclosing most of this
        // function in a "while (true){...}" loop or using goto, but they
        // are ugly. It's simpler to just recurse. The recursion will never
        // compound, and it's cheap, so we'll do it that way to be clean.
        return advance();
    case '\t':
    case ' ':
        // Are we seeing whitespace at the beginning of a line? This should
        // only happen on the first line in the file...
        if (t.substr.begin == line_begin) {
            scan_indents_and_lines();
        } else {
            // At any other place, whitespace is ignored. Skip over it.
            p = scan_spaces_and_tabs(p + 1, txt.end);
        }
        return advance();
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

inline char const * scan_spaces_and_tabs(char const * p, char const * end) {
    while (p < end) {
        char c = *p;
        if (c != ' ' && c != '\t') {
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
