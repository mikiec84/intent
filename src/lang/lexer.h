#ifndef intent_lang_lexer_h
#define intent_lang_lexer_h

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
    lexer(char const * begin, char const * end);
    lexer(char const * begin, size_t length);
    lexer(intent::core::text::str_view const & txt);

    class iterator;

    iterator begin();
    iterator end() const;

private:
    intent::core::text::str_view txt;
    token t;
    char const * line_begin;
    char const * p;
    char const * inconsistent_indent;
    uint32_t line_number;
    uint32_t total_indent_width;
    int32_t indent_dedent_delta;
    uint32_t last_stack_insert_idx;
    uint8_t indent_stack[64];

    typedef std::unique_ptr<note> note_handle_t;
    note_handle_t pending_error;

    void push_error(issue_id);
    bool advance();

    void scan_lt();
    void scan_gt();
    void scan_question();
    void scan_minus();
    void scan_plus();
    void scan_dot();
    void scan_brace();
    void scan_colon();
    void scan_star();
    void scan_slash();
    void scan_amper();
    void scan_equals();
    void scan_bang();
    void scan_pipe();
    void scan_caret();
    void scan_percent();
    void scan_backslash();
    bool scan_quote();
    void scan_hash();

    void consume(unsigned char_count, token_type tt);
    char const * get_comment_token();
    char get_indent_char() const;
    bool get_phrase_token();
    bool next_line_continues(char const * beginning_of_next_line);
    char const * get_string_literal(char c);
    char const * scan_beginning_of_line();
    void push_indent(uint32_t new_indent_width);
    uint32_t pop_indent();
};

}} // end namespace

#include "lang/lexer-inline.h"

#endif // sentry
