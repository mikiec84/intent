#ifndef _46bbaad148a24da4b41b60e28b872f15
#define _46bbaad148a24da4b41b60e28b872f15

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
