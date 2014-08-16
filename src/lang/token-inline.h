#ifndef _token_inline_h_76f2445c54594764807d34d85f7d53b7
#define _token_inline_h_76f2445c54594764807d34d85f7d53b7

#include <cfloat> // FLT_MAX, FLT_MIN
#include <cstdint> // INT32_MAX, etc
#include <cstring> // memset

#ifndef TOKEN_695ce797496541c9b74e67f9c5bfa6a1
#include "lang/token.h"
#endif

namespace intent {
namespace lang {

inline bool token::operator ==(token const & rhs) const {
    return type == rhs.type && substr == rhs.substr;
}

} // end namespace lang
} // end namespace intent

#endif // sentry
