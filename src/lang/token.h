#ifndef _695ce797496541c9b74e67f9c5bfa6a1
#define _695ce797496541c9b74e67f9c5bfa6a1

#include <memory>

#include "core/sslice.h"
#include "lang/token_type.h"

namespace intent {
namespace lang {

enum token_value_type {
    tvt_none,
    tvt_double,
    tvt_uint64_t,
    tvt_int64_t,
};

union token_value {
    double double_value;
    uint64_t uint64_t_value;
    int64_t int64_t_value;
};

/**
 * Hold information about a single token parsed out from intent code.
 */
struct token {
    token();
    token(token_type type, char const * begin, char const * end);
    token(token_type type, sslice const & slice);
    ~token();

    bool operator ==(token const & rhs) const;

    token_type type;
    sslice substr;
    token_value_type value_type;
    token_value value;
};

} // end namespace lang
} // end namespace intent

#include "lang/token-inline.h"

#endif // sentry
