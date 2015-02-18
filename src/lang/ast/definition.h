#ifndef intent_lang_ast_definition_h
#define intent_lang_ast_definition_h

#include "lang/ast/statement.h"

namespace intent {
namespace lang {
namespace ast {

/**
 * A definition is a statement that introduces a name. It may also assign a
 * value to the name.
 */
class definition : public statement {
public:

    definition(node * parent, token name, expression characterization);
    virtual ~definition();
};

}}} // end namespace

#endif // sentry
