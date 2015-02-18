#ifndef intent_lang_ast_statement_h
#define intent_lang_ast_statement_h

#include "lang/ast/node.h"

namespace intent {
namespace lang {
namespace ast {

/**
 * A statement is a single "sentence" of computer code.
 */
class statement : public node {
public:
    statement(node * parent);
    virtual ~statement();
};

}}} // end namespace

#endif // sentry
