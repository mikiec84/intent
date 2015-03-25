#ifndef _48f7eccc9c7f42a2b971fbced13c6a53
#define _48f7eccc9c7f42a2b971fbced13c6a53

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

#include "lang/token.h"
#include "lang/note.h"

namespace intent {
namespace lang {

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
    typedef intent::core::text::str_view lexable_t;

    lexer(char const * begin, char const * end);
    lexer(char const * begin, size_t length);
    lexer(lexable_t const & txt);

    /** Behaves like a pointer to a token. */
    class iterator;

    iterator begin();
    iterator end() const;

private:
    lexable_t txt;
    token t;
    // As we lex phrases, quoted strings, and comments, we want to build up a
    // string that contains a normalized version of the text, with all wrapping
    // undone. This string must have a lifetime at least as long as the token
    // state returned by an iterator, because it's stored in an arg, which takes
    // no ownership of its value. We address this by storing the unwrapped text
    // in a member variable. In the long run, what we ought to do is create a
    // string bag in the lexer (or even better, the parser), and store an index
    // into the string bag instead. This is better because the same names are
    // likely to appear over and over again, and we save space plus mutexing of
    // the heap if we don't duplicate those values repeatedly.
    std::string unwrapped_text;
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
    void scan_quote();
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
