#include <cstring>

#include "lang/token.h"

namespace intent {
namespace lang {

token::token() : type(tt_invalid), value_width(_value_width),
    value_type(_value_type), value(_value) {
    reset_value(tvt_none);
}

token::token(token_type tt, char const * begin, char const * end):
    substr(begin, end), type(tt), value_width(_value_width),
    value_type(_value_type), value(_value) {
    reset_value(tvt_none);
}

token::token(token_type tt, sslice const & slice) : substr(slice),
    type(tt), value_width(_value_width), value_type(_value_type),
    value(_value) {
    reset_value(tvt_none);
}

token::~token() {
    reset_value(tvt_none);
}

token::token(token const & other): type(tt_invalid), value_width(_value_width),
    value_type(_value_type), value(_value) {
    _value_type = tvt_none;
    *this = other;
}

token & token::operator =(token const & rhs) {
    if (rhs._value_type == tvt_txt) {
        reset_value(tvt_none);
        set_value(rhs.type, rhs.value.txt_value);
    } else {
        memcpy(&_value, &rhs._value, sizeof(_value));
        _value_type = rhs._value_type;
    }
    return *this;
}


void token::set_value(token_type tt, char const * val) {
    reset_value(tvt_txt);
    type = tt;
    if (val && *val) {
        _value.txt_value = new char[strlen(val) + 1];
        strcpy(value.txt_value, val);
    }
}

} // end namespace lang
} // end namespace intent
