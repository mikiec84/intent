#ifndef _675498d1c9f111e490dd3c15c2ddfeb4
#define _675498d1c9f111e490dd3c15c2ddfeb4

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

    core::text::str_view substr;
    token_type type;
    intent::core::text::arg value;
};

}} // end namespace

#include "lang/token-inline.h"

#endif // sentry
