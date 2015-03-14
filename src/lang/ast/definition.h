#ifndef _f0e7dd3ba5f848d5872f4ca22162cce6
#define _f0e7dd3ba5f848d5872f4ca22162cce6

#include "lang/ast/statement.h"

namespace intent {
namespace lang {
namespace ast {

/**
 * A definition is a statement that introduces a name. It may also assign a
 * value to the name.
 */
class definition : public statement {
public:

    definition(node * parent, token name, expression characterization);
    virtual ~definition();
};

}}} // end namespace

#endif // sentry
