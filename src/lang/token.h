#ifndef intent_lang_token_h
#define intent_lang_token_h

#include <cstdint>
#include <memory>
#include <boost/any.hpp>

#include "core/sslice.h"
#include "lang/token_type.h"

namespace intent {
namespace lang {

/**
 * Hold information about a single token parsed out from intent code.
 */
class token {
public:
    token();
    //token(token_type type, char const * begin, char const * end);
    token(token_type type, intent::core::text::sslice const & slice);
    ~token();

    token(token const & other);
    token & operator =(token const & rhs);

    /**
     * Tokens are considered "equal" if they have the same type and the same slice.
     */
    bool operator ==(token const & rhs) const;

    intent::core::text::sslice substr;
    token_type type;
    boost::any value;
};

}} // end namespace

#include "lang/token-inline.h"

#endif // sentry
