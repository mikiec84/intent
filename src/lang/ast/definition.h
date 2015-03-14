#ifndef _67543ccac9f111e4b6d03c15c2ddfeb4
#define _67543ccac9f111e4b6d03c15c2ddfeb4

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
