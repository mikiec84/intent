#include "lang/lexer.h"

namespace intent {
namespace lang {

char const * read_line(char const * p, char const * end);
char const * read_quoted_string(char const * p, char const * end);
char const * read_spaces(char const * p, char const * end);
char const * read_number(char const * p, char const * end, token & t);
char const * read_phrase(char const * p, char const * end);

lexer::lexer(char const * begin) :
    txt(begin), line_begin(begin), p(nullptr), line_number(1), t(tt_invalid, begin, begin) {
    advance();
}

lexer::lexer(char const * begin, char const * end) :
    txt(begin, end), line_begin(begin), p(nullptr), line_number(1), t(tt_invalid, begin, begin) {
    advance();
}

lexer::lexer(sslice const & txt) :
    txt(txt), line_begin(txt.begin), p(nullptr), line_number(1), t(tt_invalid, txt.begin, txt.begin) {
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
        t.substr.end = read_spaces(p + 1, txt.end);
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
        t.substr.end = read_line(p + 1, txt.end);
        t.type = tt_private_comment;
        break;
    case '|':
        t.substr.end = read_line(p + 1, txt.end);
        t.type = tt_doc_comment;
        break;
    case '"':
        t.substr.end = read_quoted_string(p + 1, txt.end);
        t.type = tt_quoted_string;
        break;
    case '-':
    case '+':
        if (p < txt.end - 1 && isdigit(p[1])) {
            t.substr.end = read_number(p, txt.end, t);
        } else {
            t.type = tt_invalid;
            t.substr.end = p + 1;
        }
        break;
    default:
        {
            char c = *p;
            if (isalpha(c)) {
                t.substr.end = read_phrase(p + 1, txt.end);
                t.type = tt_noun_phrase;
            } else if (isdigit(c)) {
                t.substr.end = read_number(p, txt.end, t);
            } else {
                t.type = tt_invalid;
                t.substr.end = p + 1;
            }
        }
        break;
    }
    return true;
}

inline char const * read_line(char const * p, char const * end) {
    while (p < end) {
        char c = *p;
        if (c == '\n' || c == '\r') {
            break;
        }
        ++p;
    }
    return p;
}

inline char const * read_quoted_string(char const * p, char const * end) {
    while (p < end) {
        char c = *p;
        if (c == '"') {
            break;
        } else if (c == '\\') {
            ++p;
        }
        ++p;
    }
    return p;
}

inline char const * read_spaces(char const * p, char const * end) {
    while (p < end) {
        if (*p != ' ') {
            break;
        }
        ++p;
    }
    return p;
}

char const * read_hex(char const * p, char const * end, token & t) {
    t.type = tt_hex_number;
    uint64_t & value = t.value.uint64_t_value;
    while (p < end) {
        char c = *p;
        if (c >= '0' && c <= '9') {
            value = (value * 16) + (c - '0');
        } else if (c >= 'a' && c <= 'f') {
            value = (value * 16) + 10 + (c - 'a');
        } else if (c >= 'A' && c <= 'F') {
            value = (value * 16) + 10 + (c - 'A');
        } else {
            break;
        }
        ++p;
    }
    return p;
}

char const * read_binary(char const * p, char const * end, token & t) {
    t.type = tt_binary_number;
    uint64_t & value = t.value.uint64_t_value;
    while (p < end) {
        char c = *p;
        if (c == '0') {
            value <<= 1;
        } else if (c == '1') {
            value = (value << 1) | 1;
        } else {
            break;
        }
        ++p;
    }
    return p;
}

char const * read_octal(char const * p, char const * end, token & t) {
    t.type = tt_octal_number;
    uint64_t & value = t.value.uint64_t_value;
    while (p < end) {
        char c = *p;
        if (c >= '0' && c <= '7') {
            value = (value * 8) + (c - '0');
        } else {
            break;
        }
        ++p;
    }
    return p;
}

char const * read_base10_integer(char const * p, char const * end, token & t) {
    t.type = tt_decimal_number;
    uint64_t & value = t.value.uint64_t_value;
    while (p < end) {
        char c = *p;
        if (c >= '0' && c <= '9') {
            value = (value * 10) + (c - '0');
        } else {
            break;
        }
        ++p;
    }
    return p;
}

char const * read_number(char const * p, char const * end, token & t) {
    t.type = tt_number_literal_mask;
    t.value.uint64_t_value = 0;

    char c = *p;
    //bool negative = (c == '-');
    if (c == '+' || c == '-') {
        ++p;
    }
    c = *p;
    if (c == '0') {
        if (p + 1 < end) {
            c = p[1];
            if (c == 'x' || c == 'X') {
                return read_hex(p + 2, end, t);
            } else if (c == 'b' || c == 'B') {
                return read_binary(p + 2, end, t);
            } else if (c >= '0' && c <= '7') {
                return read_octal(p + 2, end, t);
            }
        }
    }
    p = read_base10_integer(p, end, t);
 #if 0
    if (p < end) {
        c = *p;
        if (c == '.' || c == 'e' || c == 'E') {
            bool found_exponent = !(c == '.');
            t.type = tt_floating_point_number;
            t.double_value = t.uint64_t_value;
            if (negative) {
                t.double_value *= -1;
            }

        } else if (*p == 'e')
    }
#endif
    return p;
}

char const * read_phrase(char const * p, char const * end) {
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
