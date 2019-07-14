#ifndef _8769a534d4724cd999cb825636d17738
#define _8769a534d4724cd999cb825636d17738

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
