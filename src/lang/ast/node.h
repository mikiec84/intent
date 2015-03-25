#ifndef _5650cbbd17644dc0946a1cdbd05a2400
#define _5650cbbd17644dc0946a1cdbd05a2400

#include <cstdint>

namespace intent {
namespace lang {
namespace ast {

/**
 * A node is the core building block of all parsed data. An abstract syntax tree
 * consists of one or more nodes of different types, arranged in a containment
 * hierarchy.
 *
 * Nodes own their children; children maintain a back ptr to their parent, but
 * this ptr should be treated like a weak reference.
 */
class node {
    node * parent;
    node ** children;
    uint32_t child_count;
    uint32_t allocated_child_count;
public:
    node(node const &) = delete;
    node & operator =(node const &) = delete;

    node(node * parent);
    virtual ~node();

    node * get_parent();
    node const * get_parent() const;

    node * get_root();
    node const * get_root() const;

    void add_child(node *);
    uint32_t get_child_count() const;

    node * get_child(uint32_t i);
    node const * get_child(uint32_t i) const;
};

}}} // end namespace

#include "lang/ast/node-inline.h"

#endif // sentry
