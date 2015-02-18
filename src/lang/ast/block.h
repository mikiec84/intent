#ifndef intent_lang_ast_block_h
#define intent_lang_ast_block_h

#include "lang/ast/node.h"

namespace intent {
namespace lang {
namespace ast {

/**
 * A block is a sequence of one or more statements that forms a lexical scope.
 */
class block : public node {
    node * parent;
public:

    block(node * parent);
    virtual ~block();
};

}}} // end namespace

#endif // sentry
