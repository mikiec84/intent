#ifndef _arg_h_6a7fc5c4e4ac4de6856e8cbc2769f04b
#define _arg_h_6a7fc5c4e4ac4de6856e8cbc2769f04b

#include <cstdint>
#include <string>

namespace boost { namespace filesystem { class path; }}

namespace intent {
namespace core {

struct sslice;

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
#ifdef DARWIN
    arg(long value);
    arg(unsigned long value);
#endif
    arg(double value);
    arg(std::string const & str);
    arg(char const * cstr);
    arg(boost::filesystem::path const & path);
    arg(sslice const & str);
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
        vt_path,
        vt_sslice,
        vt_allocated_str,
        vt_bool
    };
    const value_type type;
    union {
        const int64_t i64;
        const uint64_t u64;
        const double dbl;
        std::string const * str;
        char const * const cstr;
        boost::filesystem::path const * path;
        sslice const * slice;
        char * allocated_str;
        const bool boolean;
    };
    ~arg();
private:
    arg();
    friend arg const & make_empty_arg();
};

}} // end namespace

#include "core/arg-inline.h"

#endif // sentry
