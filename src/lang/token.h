#ifndef _695ce797496541c9b74e67f9c5bfa6a1
#define _695ce797496541c9b74e67f9c5bfa6a1

#include <memory>

namespace intent {
namespace lang {

/**
 * Uniquely identify the semantics of a token.
 */
enum token_type {
    tt_unknown = 0,
    #define TUPLE(name, number, example, precedence, associativity, comment) \
        tt_operator##name = number,
    #include "lang/operator_tuples.h"
};

/**
 * Hold information about a single token parsed out from intent code.
 */
struct token {
    token();
    token(token_type type, char const * begin, char const * end);
    bool operator ==(token const & rhs) const;

    token_type type;
    char const * begin;
    char const * end;    
};

} // end namespace lang
} // end namespace intent

#include "lang/token-inline.h"

#endif // sentry
