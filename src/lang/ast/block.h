#ifndef _675428d1c9f111e493633c15c2ddfeb4
#define _675428d1c9f111e493633c15c2ddfeb4

#include "lang/ast/node.h"

namespace intent {
namespace lang {
namespace ast {

/**
 * A block is a sequence of one or more statements that forms a lexical scope.
 */
class block : public node {
    node * parent;
public:

    block(node * parent);
    virtual ~block();
};

}}} // end namespace

#endif // sentry
