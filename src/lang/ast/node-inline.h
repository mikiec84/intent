#ifndef _82d43b794e6e4fcb8f952b57bcec240a
#define _82d43b794e6e4fcb8f952b57bcec240a

#include "lang/ast/node.h"


namespace intent {
namespace lang {
namespace ast {


inline node::node(node * _parent): parent(_parent), children(nullptr),
    child_count(0), allocated_child_count(0) {
}


inline node * node::get_parent() {
    return parent;
}


inline node const * node::get_parent() const {
    return parent;
}


inline node * node::get_root() {
    auto p = this;
    while (true) {
        auto nxt = p->parent;
        if (!nxt) {
            return p;
        }
        p = nxt;
    }
}


inline node const * node::get_root() const {
    return const_cast<node const *>(const_cast<node *>(this)->get_root());
}


inline uint32_t node::get_child_count() const {
    return child_count;
}


inline node const * node::get_child(uint32_t i) const {
    return i < child_count ? children[i] : nullptr;
}


inline node * node::get_child(uint32_t i) {
    return i < child_count ? children[i] : nullptr;
}


}}} // end namespace


#endif // sentry
