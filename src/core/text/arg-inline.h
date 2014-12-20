#ifndef intent_core_arg_inline_h
#define intent_core_arg_inline_h

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
    type(vt_str), str(&value) {
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

}}} // end namespace

#endif // sentry
