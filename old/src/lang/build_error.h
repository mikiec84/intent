#ifndef _7cf17e3931cd4e97a4a2709382ba15ac
#define _7cf17e3931cd4e97a4a2709382ba15ac

#include <stdexcept>

namespace intent {
namespace lang {

struct build_error: public std::runtime_error {
	build_error(char const * file, unsigned line, unsigned column, char const * msg);
};

}} // end namespace

#include "lang/token_type-inline.h"

#endif // sentry
