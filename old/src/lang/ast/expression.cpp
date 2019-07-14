#include "lang/ast/expression.h"

namespace intent {
namespace lang {
namespace ast {

expression::~expression() {
}

token const & bad_token = *((token *)nullptr);

token const & expression::first_token() const {
    return bad_token;
}

token const & expression::last_token() const {
    return bad_token;
}


}}} // end namespace
