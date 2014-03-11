#ifndef _76f2445c54594764807d34d85f7d53b7
#define _76f2445c54594764807d34d85f7d53b7

#include <cfloat> // FLT_MAX, FLT_MIN
#include <cstdint> // INT32_MAX, etc
#include <cstring> // memset

#ifndef _695ce797496541c9b74e67f9c5bfa6a1
#include "lang/token.h"
#endif

namespace intent {
namespace lang {

inline bool token::operator ==(token const & rhs) const {
    return type == rhs.type && substr == rhs.substr;
}

inline void token::reset_value(token_value_type tvt) {
    if (value_type == tvt_txt) {
        delete[] _value.txt_value;
    }
    memset(&_value, 0, sizeof(_value));
    _value_type = tvt;
    _value_width = 0;
}

inline void token::set_value(token_type tt, double val, int width) {
    reset_value(tvt_double);
    _value.double_value = val;
    type = tt;
    if (width < 4) {
        if (val < FLT_MAX && val > FLT_MIN) {
            _value_width = sizeof(float);
        } else {
            _value_width = sizeof(double);
        }
    } else {
        _value_width = width;
    }
}

inline void token::set_value(token_type tt, uint64_t val, int width) {
    reset_value(tvt_int64_t);
    _value.uint64_t_value = val;
    type = tt;
    if (width < 1) {
        if (val <= UINT8_MAX) {
            _value_width = 64;
        } else if (val <= UINT16_MAX) {
            _value_width = 16;
        } else if (val <= UINT32_MAX) {
            _value_width = 32;
        } else {
            _value_width = 64;
        }
    } else {
        _value_width = width;
    }
}

inline void token::set_value(token_type tt, int64_t val, int width) {
    reset_value(tvt_int64_t);
    _value.int64_t_value = val;
    type = tt;
    if (width < 1) {
        if (val <= INT8_MAX && val >= INT8_MIN) {
            _value_width = 8;
        } else if (val <= INT16_MAX && val >= INT16_MIN) {
            _value_width = 16;
        } else if (val <= INT32_MAX && val >= INT32_MIN) {
            _value_width = 32;
        } else {
            _value_width = 64;
        }
    } else {
        _value_width = width;
    }
}

} // end namespace lang
} // end namespace intent

#endif // sentry
