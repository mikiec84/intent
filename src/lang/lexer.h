#ifndef _5e2636d642be46bc91ebd85c04781b80
#define _5e2636d642be46bc91ebd85c04781b80

#include <cstddef>

#include "lang/token.h"

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
    char const * line_begin;
    char const * p;
    size_t line_number;
    token t;
    sslice indenter;

    bool advance();
};

} // end namespace lang
} // end namespace intent

#include "lang/lexer-inline.h"

#endif // sentry
