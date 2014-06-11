#ifndef ARGINLINE_H
#define ARGINLINE_H

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

inline arg::arg(long value) :
    type(vt_signed), i64(value) {
}

inline arg::arg(unsigned long value) :
    type(vt_unsigned), u64(value) {
}

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

inline arg::arg(sslice const & txt) :
    type(vt_sslice), slice(&txt) {
}

inline arg::arg(bool value) :
    type(vt_bool), boolean(value) {
}

#endif // ARGINLINE_H
