#include <cmath>
#include <string>

#include "core/scan_numbers.h"
#include "core/strutil.h"
#include "core/unicode.h"
#include "lang/lexer.h"

using std::string;

namespace intent {
namespace lang {

bool is_line_break(char c);
char const * scan_rest_of_line(char const * p, char const * end);
bool get_number_token(char const * p, char const * end, token & t);
char const * scan_phrase(char const * p, char const * end);

const uint32_t NO_INDENT_YET = UINT32_MAX;
const uint32_t EMPTY_INDENT_STACK = 0;

lexer::lexer(char const * begin) : lexer(begin, begin ? strchr(begin, 0) : nullptr) {
}

lexer::lexer(sslice const & txt) : lexer(txt.begin, txt.end) {
}

lexer::lexer(char const * begin, char const * end) :
    txt(begin, end), line_begin(begin), p(begin), inconsistent_indent(nullptr),
    statement_number(0), line_number(0), total_indent_width(0),
    indent_dedent_delta(0), last_stack_insert_idx(NO_INDENT_YET) {

    if (p) {
        // Consume any whitespace at the beginning of the code. We always call
        // scan_beginning_of_line() immediately after a line break, to consume
        // indents and make sure advance() doesn't have to worry about them.
        // The beginning of the code is like any other line break, as far as
        // indenting is concerned, so we should do the same thing here.
        t.substr.end = scan_beginning_of_line();
        advance();
    }
}

inline uint32_t get_indent_width(char const * p, char const * end,
                                 char indent_char, char const * & inconsistent) {
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

/**
 * Call this each time we find a line break in the code. This lets us do housekeeping
 * to indent and dedent, to keep track of which line number we're on, and to suppress
 * whitespace tokens that are irrelevant.
 *
 * @pre p points to the first char after \n
 * @post p points to the first char that's not indenting whitespace, at the beginning
 *     of a new line--not necessarily the same line that we started with. line_number
 *     and indents have been updated appropriately.
 */
char const * lexer::scan_beginning_of_line() {
    while (p < txt.end) {
        ++line_number;
        line_begin = p;
        char const * end_of_indent = scan_spaces_and_tabs(p, txt.end);
        if (end_of_indent == txt.end) {
            return txt.end;
        }
        p = end_of_indent;
        char c = *p;
        if (is_line_break(c)) {
            p = consume_line_break(p, txt.end);
        } else {
            // Found a line that's not empty. Look at its indent.
            uint32_t indent_width = get_indent_width(line_begin, end_of_indent,
                                                get_indent_char(), inconsistent_indent);
            if (indent_width > total_indent_width) {
                if (last_stack_insert_idx == NO_INDENT_YET) {
                    indent_stack[0] = *line_begin;
                    last_stack_insert_idx = EMPTY_INDENT_STACK;
                }
                push_indent(indent_width - total_indent_width);
            } else if (indent_width < total_indent_width) {
                indent_dedent_delta = -1 * static_cast<int>(
                            total_indent_width - indent_width);
            }
            break;
        }
    }
    return p;
}

void lexer::push_error(issue_id issue) {
    pending_error = note_handle_t(new note(issue, nullptr, line_number, p - line_begin));
}


uint32_t lexer::pop_indent() {
    unsigned width = 0;
    if (last_stack_insert_idx > EMPTY_INDENT_STACK) {
        width = indent_stack[last_stack_insert_idx--];
        total_indent_width -= width;
    }
    return width;
}

inline void lexer::scan_lt() {
    if (p + 1 < txt.end) {
        switch (p[1]) {
        case '<':
            if (p + 2 < txt.end && p[2] == '=') {
                p += 3;
                t.type = tt_operator_lshift_equals;
            } else {
                p += 2;
                t.type = tt_operator_lshift;
            }
            return;
        case '=':
            if (p + 2 < txt.end && p[2] == '>') {
                if (p + 3 < txt.end && p[3] == '?') {
                    t.type = tt_operator_spaceshipq;
                    p += 4;
                } else {
                    t.type = tt_operator_spaceship;
                    p += 3;
                }
                return;
            }
            p += 2;
            t.type = tt_operator_less_equal;
            return;
        default:
            break;
        }
    }
    ++p;
    t.type = tt_operator_less;
}

inline void lexer::scan_gt() {
    if (p + 1 < txt.end) {
        switch (p[1]) {
        case '>':
            if (p + 2 < txt.end && p[2] == '=') {
                p += 3;
                t.type = tt_operator_rshift_equals;
            } else {
                p += 2;
                t.type = tt_operator_rshift;
            }
            return;
        case '=':
            p += 2;
            t.type = tt_operator_greater_equal;
            return;
        default:
            break;
        }
    }
    ++p;
    t.type = tt_operator_greater;
}

inline void lexer::scan_minus() {
    if (p + 1 < txt.end) {
        switch (p[1]) {
        case '>':
            p += 2;
            t.type = tt_operator_cast;
            return;
        case '=':
            p += 2;
            t.type = tt_operator_minus_equals;
            return;
        case '?':
            p += 2;
            t.type = tt_operator_implied_negative_mark;
            return;
        default:
            if (isdigit(p[1])) {
                get_number_token(p, txt.end, t);
                return;
            }
            break;
        }
    }
    ++p;
    t.type = tt_operator_minus;
}

inline void lexer::scan_plus() {
    if (p + 1 < txt.end) {
        switch (p[1]) {
        case '=':
            p += 2;
            t.type = tt_operator_plus_equals;
            return;
        case '?':
            p += 2;
            t.type = tt_operator_implied_positive_mark;
            return;
        default:
            if (isdigit(p[1])) {
                get_number_token(p, txt.end, t);
                return;
            }
            break;
        }
    }
    ++p;
    t.type = tt_operator_plus;
}

inline void lexer::scan_q() {
    if (p + 1 < txt.end) {
        switch (p[1]) {
        case '.':
            p += 2;
            t.type = tt_operator_safe_dot;
            return;
        case '[':
            if (p + 2 > txt.end && p[2] == '?') {
                t.type = tt_operator_safe_subscript_safe_empty;
                p += 3;
                return;
            }
            t.type = tt_operator_safe_subscript;
            p += 2;
            return;
        case ':':
            t.type = tt_operator_elvis;
            p += 2;
            return;
        case '<':
            if (p + 3 > txt.end && p[2] == '=' && p[3] == '>') {
                t.type = tt_operator_qspaceship;
                p += 4;
                return;
            }
            break;
        default:
            break;
        }
    }
    ++p;
    t.type = tt_operator_greater;
}

bool lexer::advance() {

    // Reset state so we report invalid, zero-width token unless/until
    // we report otherwise.
    t.type = tt_none;
    t.value = 0;
    p = t.substr.begin = t.substr.end;

    // Before we scan more text, emit any errors that we've already discovered.
    if (pending_error) {
        t.type = tt_error;
        t.value = pending_error->to_string();
        pending_error.reset();
        return true;
    }

    // Also before we scan more text, pop any indents/dedents that are queued up.
    if (indent_dedent_delta) {
        if (indent_dedent_delta > 0) {
            indent_dedent_delta = 0;
            t.type = tt_indent;
            return true;
        } else {
            indent_dedent_delta += pop_indent();
            if (indent_dedent_delta > 0) {
                indent_dedent_delta = 0;
                    // TODO: issue error
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
        if (p + 1 < txt.end && p[1] == '\n') ++p;
        // fall through; don't break
    case '\n':
        if (++p < txt.end) {
            // Find something besides new lines to look at.
            t.substr.end = scan_beginning_of_line();
        } else {
            t.substr.end = txt.end;
            // Force final dedents.
            indent_dedent_delta = -1 * static_cast<int>(total_indent_width);
        }
        // What we need to do now is react to new knowledge we may have
        // about indents and dedents--or to react to some character besides
        // a line break. There are ways to do this by enclosing most of this
        // function in a "while (true){...}" loop or using goto, but they
        // are ugly. It's simpler to just recurse. The recursion will never
        // compound, and it's cheap, so we'll do it that way to be clean.
        return advance();
    case '\t':
    case ' ':
        p = scan_spaces_and_tabs(p + 1, txt.end);
        return advance();
    case '<':
        scan_lt();
        break;
    case '>':
        scan_gt();
        break;
    case '?':
        scan_q();
        break;
    case '[':
        if (p + 1 < txt.end && p[1] == '?') {
            p += 2;
            t.type = tt_operator_safe_empty;
        } else {
            ++p;
            t.type = tt_operator_subscript;
        }
        break;
    case ']':
        ++p;
        t.type = tt_operator_subscript_end;
    case '(':
        t.type = tt_operator_group;
        ++p;
        return true;
    case ')':
        t.type = tt_operator_ungroup;
        ++p;
        return true;
    case '#':
        t.type = tt_private_comment;
        // Do not break
    case '|':
        if (t.type == tt_none) {
            t.type = tt_doc_comment;
        }
        t.substr.end = get_comment_token();
        break;
    case '"':
        get_quoted_string_token();
        break;
    case '-':
        scan_minus();
        break;
    case '+':
        scan_plus();
        break;
    default:
        {
            char c = *p;
            if (isalpha(c)) {
                get_phrase_token();
            } else if (isdigit(c)) {
                get_number_token(p, txt.end, t);
            } else {
                t.type = tt_error;
                t.substr.end = p + 1;
            }
        }
        break;
    }
    return true;
}

/**
 * @pre p points at first char of phrase
 * @post p points to first char after phrase
 */
bool lexer::get_phrase_token() {
    string value;
    t.type = isupper(*p) ? tt_verb_phrase : tt_noun_phrase;
    for (; p != txt.end; ++p) {
        char c = *p;
        if (isalnum(c) || c == '\'' || c == ' ') {
            value += c;
        } else if (is_line_break(c)) {
            if (next_line_continues(consume_line_break(p, txt.end))) {
                value += ' ';
            }
            break;
        } else {
            break;
        }
    }
    t.value = value;
    return true;
}



bool lexer::next_line_continues(char const * beginning_of_next_line) {
    auto last_line_num = line_number;
    auto last_indent = total_indent_width;
    p = beginning_of_next_line;
    auto next = scan_beginning_of_line();
    if (last_line_num + 1 != line_number || last_indent != total_indent_width) {
        return false;
    }
    if (next + 3 >= txt.end || strncmp(next, "...", 3) != 0) {
        return false;
    }
    p = next + 3;
    return true;
}

char const * lexer::get_comment_token() {
    string value;
    while (true) {
        ++p;
        auto end_of_line = scan_rest_of_line(p, txt.end);
        auto after_spaces = scan_spaces_and_tabs(p, txt.end);
//        if (after_spaces > p) {
//            if (*p == ' ' && p)
//        }
        if (after_spaces < end_of_line) {
            auto rtrim_result = rtrim(after_spaces, end_of_line);
            if (!value.empty()) {
                value += ' ';
            }
            value += string(after_spaces, rtrim_result);
        }
        if (!next_line_continues(end_of_line + 1)) {
            break;
        }
    }
    t.value = value;
    return p;
}

inline char const * scan_rest_of_line(char const * p, char const * end) {
    while (p < end) {
        if (is_line_break(*p)) {
            break;
        }
        ++p;
    }
    return p;
}

/**
 * @pre p points at first content char inside quoted str
 * @return next char worth analyzing, after close quote. This may be the
 *     char immediately after the close quote, or it may be a few chars
 *     later if the quoted string ended unnaturally and we analyzed line
 *     breaks without seeing a continuation.
 * @post t.value contains the normalized content of quoted str
 */
char const * lexer::scan_quoted_string() {
    string value;
    while (p < txt.end) {
        bool handled = false;
        char c = *p;
        if (c == '"') {
            ++p;
            break;
        } else if (c == '\\') {
            codepoint_t cp;
            // TODO: tweak scan_unicode_escape_sequence() so it honors end.
            char const * seq_end = scan_unicode_escape_sequence(p + 1, cp);
            if (cp != UNICODE_REPLACEMENT_CHAR && seq_end > p && seq_end <= txt.end) {
                handled = true;
                char buf[16];
                size_t buf_length = sizeof(buf);
                char * tmp = buf;
                cat_codepoint_to_utf8(tmp, buf_length, cp);
                // TODO: prove that cat_codepoint_to_utf8 worked.
                value += buf;
                p = seq_end;
            }
        } else if (is_line_break(c)) {
            if (next_line_continues(consume_line_break(p, txt.end))) {
                c = ' ';
            } else {
                push_error(ii_unterminated_string_literal);
                break;
            }
        }
        if (!handled) {
            value += c;
            ++p;
        }
    }
    t.value = value;
    return p;
}

/**
 * @pre p points at opening quote char
 * @post p points to first char after terminating quote
 * @return true if able to get quoted string
 */
bool lexer::get_quoted_string_token() {
    t.type = tt_quoted_string;
    ++p;
    t.substr.end = scan_quoted_string();
    return true;
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
                token_type tt = tt_none;
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
                if (tt != tt_none) {
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

inline bool is_line_break(char c) {
    return c == '\n' || c == '\r';
}

} // end namespace lang
} // end namespace intent
