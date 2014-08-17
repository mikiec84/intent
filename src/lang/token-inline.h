#ifndef intent_lang_token_inline_h
#define intent_lang_token_inline_h

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

}} // end namespace

#endif // sentry
