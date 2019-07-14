#ifndef _d25f999cf336433a98ed620335129463
#define _d25f999cf336433a98ed620335129463

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
