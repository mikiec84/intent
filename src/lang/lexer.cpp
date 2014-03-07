#include "lang/lexer.h"

namespace intent {
namespace lang {

char const * read_line(char const * p, char const * end);
char const * read_quoted_string(char const * p, char const * end);
char const * read_spaces(char const * p, char const * end);
char const * read_number(char const * p, char const * end);
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

    if (t.substr.end >= txt.end) return false;

    switch (*p) {
    case 0:
        return false;
    case '\r':
        if (p[1] == '\n') ++p;
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
    default:
        {
            char c = *p;
            if (isalpha(c)) {
                t.substr.end = read_phrase(p + 1, txt.end);
            } else if (isdigit(c)) {
                t.substr.end = read_number(p + 1, txt.end);
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

char const * read_number(char const * p, char const * end) {
    while (p < end) {
        if (!isdigit(*p)) {
            break;
        }
        ++p;
    }
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
