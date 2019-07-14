#ifndef _7eeceecbc15042a38c5ed88819796c1a
#define _7eeceecbc15042a38c5ed88819796c1a

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
