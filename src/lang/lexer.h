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
    lexer(char const * txt);
    lexer(char const * txt, char const * txt_end);

    class iterator;

    iterator begin();
    iterator end() const;

private:
    char const * txt;
    char const * txt_end;
    size_t line_number;
    token t;

    bool advance();

    bool read_number();
    bool read_quoted_string();
};

} // end namespace lang
} // end namespace intent

#include "lang/lexer-inline.h"

#endif // sentry
