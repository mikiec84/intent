#ifndef _ae01d21815404de6938cf86ec80fb83e
#define _ae01d21815404de6938cf86ec80fb83e

#ifndef _695ce797496541c9b74e67f9c5bfa6a1
#include "lang/token.h"
#endif

namespace intent {
namespace lang {

inline token::token() : type(tt_invalid), substr() {
}

inline token::token(token_type type, char const * begin, char const * end) :
    type(type), substr(begin, end) {
}

inline token::token(token_type type, sslice const & substr) :
    type(type), substr(substr) {
}

inline bool token::operator ==(token const & rhs) const {
    return type == rhs.type && substr == rhs.substr;
}

} // end namespace lang
} // end namespace intent

#endif
