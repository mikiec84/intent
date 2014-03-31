#ifndef ARG_H
#define ARG_H

#include <cstdint>
#include <string>

/**
 * An arg is a specialized type of variant--a lightweight wrapper around
 * any argument passed to interp() or similar functions that need variadic
 * behavior plus type safety, without using C++ 11's arglist.
 *
 * Unlike boost::any or lexical_cast, this class is only guaranteed to behave
 * properly as long as its lifetime is less than the lifetime of the item it
 * wraps. In exchange for this guarantee, arg is faster and simpler to use--
 * you basically pretend it doesn't exist, and it's transparent and nearly
 * free.
 */
struct arg {
    arg(int64_t value);
    arg(uint64_t value);
    arg(int32_t value);
    arg(uint32_t value);
    arg(int16_t value);
    arg(uint16_t value);
    arg(int8_t value);
    arg(uint8_t value);
    arg(long value);
    arg(unsigned long value);
    arg(double value);
    arg(std::string const & str);
    arg(char const * cstr);
    arg(bool);

    /**
     * Write at most buflen bytes to buf, always finishing with trailing null.
     * @return strlen() of string written, or number of bytes needed in buf
     *     if its size is inadequate. Same semantics as posix snprintf(). Any
     *     number >= buflen represents an overflow.
     */
    int snprintf(char * buf, size_t buflen, char const * format = nullptr) const;
    std::string to_string(char const * format = nullptr) const;

    static arg const & empty;

    enum value_type {
        vt_empty,
        vt_signed,
        vt_unsigned,
        vt_float,
        vt_date,
        vt_str,
        vt_cstr,
        vt_bool
    };
    const value_type type;
    union {
        const int64_t i64;
        const uint64_t u64;
        const double dbl;
        std::string const * str;
        char const * const cstr;
        const bool boolean;
    };
private:
    arg();
    friend arg const & make_empty_arg();
};

#include "core/arg-inline.h"

#endif // ARG_H
