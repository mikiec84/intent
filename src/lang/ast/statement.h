#ifndef _67545cc7c9f111e4968c3c15c2ddfeb4
#define _67545cc7c9f111e4968c3c15c2ddfeb4

#include "lang/ast/node.h"

namespace intent {
namespace lang {
namespace ast {

/**
 * A statement is a single "sentence" of computer code.
 */
class statement : public node {
public:
    statement(node * parent);
    virtual ~statement();
};

}}} // end namespace

#endif // sentry
