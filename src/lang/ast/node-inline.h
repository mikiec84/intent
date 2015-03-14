#ifndef _82d43b794e6e4fcb8f952b57bcec240a
#define _82d43b794e6e4fcb8f952b57bcec240a

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
