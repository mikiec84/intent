#ifndef _ae01d21815404de6938cf86ec80fb83e
#define _ae01d21815404de6938cf86ec80fb83e

#ifndef _695ce797496541c9b74e67f9c5bfa6a1
#include "lang/token.h"
#endif

namespace intent {
namespace lang {

inline token::token() : type(tt_unknown), begin(nullptr), end(nullptr) {
}

inline token::token(token_type type, char const * begin, char const * end) :
    type(type), begin(begin), end(end) {
}

inline bool token::operator ==(token const & rhs) const {
    return type == rhs.type && begin == rhs.begin && end == rhs.end;
}

} // end namespace lang
} // end namespace intent

#endif
