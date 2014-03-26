#include <cstring>

#include "lang/token.h"

namespace intent {
namespace lang {

token::token() : type(tt_null) {
}

token::~token() {
}

token::token(token const & other) {
    *this = other;
}

token & token::operator =(token const & rhs) {
    substr = rhs.substr;
    type = rhs.type;
    value = rhs.value;
    return *this;
}

} // end namespace lang
} // end namespace intent
