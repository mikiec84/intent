#ifndef intent_lang_ast_conditional_h
#define intent_lang_ast_conditional_h

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
