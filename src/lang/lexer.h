#ifndef _5e2636d642be46bc91ebd85c04781b80
#define _5e2636d642be46bc91ebd85c04781b80

#include <cstddef>

#include "lang/token.h"

namespace intent {
namespace lang {

class token;

/**
 * Tokenize a chunk of intent code.
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
