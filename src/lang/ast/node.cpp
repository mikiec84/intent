#include "lang/ast/expression.h"

#include <cstdlib>

namespace intent {
namespace lang {
namespace ast {

void node::add_child(handle_t child) {
    // We could use a std::vector<node *> instead. However, I don't want to
    // preallocate *any* space, and I only want to allocate enough space for
    // 4 children in the common case, because most nodes have very few children.
    // Maybe we should write a custom allocator and use it with std::vector?
    if (!child) {
        return;
    }
    static constexpr auto unit_size = sizeof(node *);
    if (child_count == allocated_child_count) {
        auto new_count = allocated_child_count ? allocated_child_count * 2 : 4;
        auto new_children = realloc(children, new_count * unit_size);
        if (new_children == nullptr) {
            throw std::bad_alloc();
        }
        children = reinterpret_cast<node **>(new_children);
        allocated_child_count = new_count;
    }
    children[child_count++] = child.get();
}


node::~node() {
    if (children) {
        for (size_t i = 0; i < child_count; ++i) {
            delete children[i];
        }
        free(children);
        children = nullptr;
    }
}

}}} // end namespace
