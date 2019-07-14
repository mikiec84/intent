#ifndef _e3a2a6fbcd514eeda6d018a78efa003e
#define _e3a2a6fbcd514eeda6d018a78efa003e

#include "lang/ast/node.h"

namespace intent {
namespace lang {
namespace ast {

/**
 * A value is any "raw" value in code, such as a numeric or string literal.
 */
class value : public {
public:
    node(node * parent);
    virtual ~node();

    node * get_parent();
    node const * get_parent() const;
};

}}} // end namespace

#include "lang/ast/node-inline.h"

#endif // sentry
