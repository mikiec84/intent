#ifndef intent_lang_ast_node_inline_h
#define intent_lang_ast_node_inline_h

#include "lang/ast/node.h"


namespace intent {
namespace lang {
namespace ast {


inline node::node(node * parent): parent(parent) {
}


inline node * node::get_parent() {
    return parent;
}


inline node const * node::get_parent() const {
    return parent;
}


}}} // end namespace


#endif // sentry
