#ifndef _0de674abe9214d818d6f5c6fb743c8d4
#define _0de674abe9214d818d6f5c6fb743c8d4

#include <cstdint>
#include <memory>

#include "core/text/arg.h"
#include "core/text/str_view.h"
#include "lang/token_type.h"

namespace intent {
namespace lang {

/**
 * Hold information about a single token parsed out from intent code.
 */
class token {
public:
	token();
	token(token_type type, intent::core::text::str_view const & slice);
	~token();

	token(token const & other);
	token & operator =(token const & rhs);

	/**
	 * Tokens are considered "equal" if they have the same type and the same slice.
	 */
	bool operator ==(token const & rhs) const;

	token_type type;
	core::text::str_view substr;
	intent::core::text::arg value;
};

}} // end namespace

#include "lang/token-inline.h"

#endif // sentry
