#ifndef _67545021c9f111e4b3f53c15c2ddfeb4
#define _67545021c9f111e4b3f53c15c2ddfeb4

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
