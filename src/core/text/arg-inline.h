#ifndef _67533263c9f111e4b64f3c15c2ddfeb4
#define _67533263c9f111e4b64f3c15c2ddfeb4

#include "core/text/arg.h"

namespace intent {
namespace core {
namespace text {

inline arg::arg(int64_t value) :
    type(vt_signed), i64(value) {
}

inline arg::arg(uint64_t value) :
    type(vt_unsigned), u64(value) {
}

inline arg::arg(int32_t value) :
    type(vt_signed), i64(value) {
}

inline arg::arg(uint32_t value) :
    type(vt_unsigned), u64(value) {
}

inline arg::arg(int16_t value) :
    type(vt_signed), i64(value) {
}

inline arg::arg(uint16_t value) :
    type(vt_unsigned), u64(value) {
}

inline arg::arg(int8_t value) :
    type(vt_signed), i64(value) {
}

inline arg::arg(uint8_t value) :
    type(vt_unsigned), u64(value) {
}

#ifdef DARWIN
inline arg::arg(long value) :
    type(vt_signed), i64(value) {
}

inline arg::arg(unsigned long value) :
    type(vt_unsigned), u64(value) {
}
#endif

inline arg::arg(double value) :
    type(vt_float), dbl(value) {
}

inline arg::arg(std::string const & value) :
    type(vt_string), str(&value) {
}

inline arg::arg(char const * value) :
    type(vt_cstr), cstr(value) {
}

inline arg::arg(boost::filesystem::path const & p) :
    type(vt_path), path(&p) {
}

inline arg::arg(str_view const & txt) :
    type(vt_str_view), slice(&txt) {
}

inline arg::arg(bool value) :
    type(vt_bool), boolean(value) {
}

inline arg::arg(arg const & other) :
    type(other.type), cstr(other.cstr) {
}

inline arg & arg::operator =(arg const & rhs) {
    type = rhs.type;
    cstr = rhs.cstr;
    return *this;
}

inline arg::arg(arg && other) :
    type(other.type), cstr(other.cstr) {
}

inline arg & arg::operator =(arg && rhs) {
    type = rhs.type;
    cstr = rhs.cstr;
    return *this;
}

inline arg::~arg() {
}

inline arg::arg() : type(vt_empty), cstr(nullptr) {
}

}}} // end namespace

#endif // sentry
