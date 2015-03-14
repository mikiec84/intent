#ifndef _5650cbbd17644dc0946a1cdbd05a2400
#define _5650cbbd17644dc0946a1cdbd05a2400

#include <memory>

namespace intent {
namespace lang {
namespace ast {

/**
 * A node is the core building block of all parsed data. An abstract syntax tree
 * consists of one or more nodes of different types, arranged in a hierarchy.
 *
 * Nodes own their children; children maintain a back ptr to their parent, but
 * this ptr should be treated like a weak reference.
 */
class node {
    node * parent;
public:
    typedef std::unique_ptr<node> node_handle;

    node(node * parent);
    virtual ~node();

    node * get_parent();
    node const * get_parent() const;
};

}}} // end namespace

#include "lang/ast/node-inline.h"

#endif // sentry
