#ifndef LEXER_5e2636d642be46bc91ebd85c04781b80
#define LEXER_5e2636d642be46bc91ebd85c04781b80

#include <cstddef>
#include <cstdint>
#include <memory>

#include "lang/token.h"
#include "lang/note.h"

namespace intent {
namespace lang {

class token;

/**
 * Tokenize a chunk of intent code.
 *
 * Tokens can be accessed by iterating from begin() to end(). All iterators on
 * a given lexer point to the lexer's current token. This means that each call
 * to increment an iterator alters the internal state of the lexer, and the
 * internal state of all iterators on the lexer; if you want to preserve info
 * about a token across iteration changes, copy the value returned by
 * dereferencing the iterator; do not save the iterator itself. This is not
 * canonical behavior for a forward iterator, but tokenization is intended to
 * be a single-pass, forward-only operation, with "diff" and similar semantics
 * precluded. Plus it simplifies our implementation and makes it faster...
 *
 * Lexers are inherently stateful. They are not thread-safe. However, multiple
 * threads can be tokenizing with different lexers, in parallel.
 */
class lexer {
public:
    lexer(char const * begin);
    lexer(char const * begin, char const * end);
    lexer(sslice const & txt);

    class iterator;

    iterator begin();
    iterator end() const;

private:
    sslice txt;
    token t;
    char const * line_begin;
    char const * p;
    char const * inconsistent_indent;
    char const * error_msg;
    uint32_t statement_number;
    uint32_t line_number;
    uint32_t total_indent_width;
    int32_t indent_dedent_delta;
    uint32_t last_stack_insert_idx;
    uint8_t indent_stack[64];

    typedef std::unique_ptr<note> note_handle_t;
    note_handle_t pending_error;

    void push_error(issue_id);
    bool advance();
    char const * get_comment_token();
    char get_indent_char() const;
    bool get_phrase_token();
    bool get_quoted_string_token();
    bool next_line_continues(char const * beginning_of_next_line);
    char const * scan_quoted_string();
    char const * scan_beginning_of_line();
    void push_indent(uint32_t new_indent_width);
    uint32_t pop_indent();
};

} // end namespace lang
} // end namespace intent

#include "lang/lexer-inline.h"

#endif // sentry
