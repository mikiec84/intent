#ifndef _67543611c9f111e495993c15c2ddfeb4
#define _67543611c9f111e495993c15c2ddfeb4

#include "lang/ast/expression.h"
#include "lang/ast/block.h"

namespace intent {
namespace lang {
namespace ast {

/**
 * A conditional is a statement that introduces a block that will execute if
 * a boolean condition is true.
 */
class conditional : public statement {
public:
    conditional(node * parent, expression condition, block dependent_block);
    virtual ~conditional();
};

}}} // end namespace

#endif // sentry
