#ifndef _695ce797496541c9b74e67f9c5bfa6a1
#define _695ce797496541c9b74e67f9c5bfa6a1

#include <memory>

#include "core/sslice.h"
#include "lang/token_type.h"

namespace intent {
namespace lang {

/**
 * Hold information about a single token parsed out from intent code.
 */
struct token {
    token();
    token(token_type type, char const * begin, char const * end);
    token(token_type type, sslice const & slice);
    bool operator ==(token const & rhs) const;

    token_type type;
    sslice substr;
};

} // end namespace lang
} // end namespace intent

#include "lang/token-inline.h"

#endif // sentry
