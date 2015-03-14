#ifndef _6754222ec9f111e4a9423c15c2ddfeb4
#define _6754222ec9f111e4a9423c15c2ddfeb4

#include "lang/ast/expression.h"
#include "lang/ast/block.h"

namespace intent {
namespace lang {
namespace ast {

/**
 * A conditional is a statement that introduces a block that will execute if
 * a boolean condition is true.
 */
class assignment : public statement {
public:
    assignment(node * parent, token target, token_type op, expression source);
    virtual ~assignment();
};

}}} // end namespace

#endif // sentry
