#ifndef _695ce797496541c9b74e67f9c5bfa6a1
#define _695ce797496541c9b74e67f9c5bfa6a1

#include <cstdint>
#include <memory>

#include "core/sslice.h"
#include "lang/token_type.h"

namespace intent {
namespace lang {

enum token_value_type {
    tvt_none,
    tvt_double,
    tvt_float,
    tvt_uint64_t,
    tvt_int64_t,
    tvt_txt,
};

union token_value {
    double double_value;
    float float_value;
    uint64_t uint64_t_value;
    int64_t int64_t_value;
    char * txt_value;
};

/**
 * Hold information about a single token parsed out from intent code.
 */
class token {
public:
    token();
    token(token_type type, char const * begin, char const * end);
    token(token_type type, sslice const & slice);
    ~token();

    token(token const & other);
    token & operator =(token const & rhs);

    /**
     * Tokens are considered "equal" if they have the same type and the same slice.
     */
    inline bool operator ==(token const & rhs) const;

    sslice substr;
    token_type type;

    // value, value_width, and value_type are not designed to be manipulated directly.
    // Instead, they are written by calling set_value(), and read via their const
    // aliases.
    void set_value(token_type tt, char const * value);
    void set_value(token_type tt, double value, int width=0);
    void set_value(token_type tt, uint64_t value, int width=0);
    void set_value(token_type tt, int64_t value, int width=0);

    int const & value_width;
    token_value_type const & value_type;
    token_value const & value;

private:
    void reset_value(token_value_type tvt);

    int _value_width;
    token_value_type _value_type;
    token_value _value;
};

} // end namespace lang
} // end namespace intent

#include "lang/token-inline.h"

#endif // sentry
