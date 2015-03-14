#ifndef _3de6033ffc4c417491619fa558d4e09b
#define _3de6033ffc4c417491619fa558d4e09b

#include <cstdint>
#include <string>

#include "core/text/str_view-fwd.h"

namespace boost { namespace filesystem { class path; }}

namespace intent {
namespace core {
namespace text {

/**
 * An arg is a specialized type of variant--a lightweight wrapper around
 * any argument passed to interp() or similar functions that need variadic
 * behavior plus type safety.
 *
 * Unlike other variant types, this class doesn't "own" anything; if it receives
 * any non-POD value, it stores a pointer to it. Thus it only behaves properly
 * as long as its lifetime is less than the lifetime of the item it wraps. In
 * exchange for this guarantee, arg is lighter, faster, and simpler to use--
 * you basically pretend it doesn't exist, and it's transparent and nearly
 * free.
 */
struct arg {
    arg(int64_t);
    arg(uint64_t);
    arg(int32_t);
    arg(uint32_t);
    arg(int16_t);
    arg(uint16_t);
    arg(int8_t);
    arg(uint8_t);
#ifdef DARWIN
    arg(long);
    arg(unsigned long);
#endif
    arg(double);
    arg(std::string const &);
    arg(char const *);
    arg(boost::filesystem::path const &);
    arg(str_view const &);
    arg(bool);

    // Support copy and move semantics. These are generally unnecessary, since
    // args are designed to be short-lived objects created just in time--but
    // allow transitive assignment (e.g., as a member of a struct).
    arg(arg const &);
    arg & operator =(arg const &);

    arg(arg &&);
    arg & operator =(arg &&);

    /**
     * Write at most buflen bytes to buf, always finishing with trailing null.
     * @return strlen() of string written, or number of bytes needed in buf
     *     if its size is inadequate. Same semantics as posix snprintf(). Any
     *     number >= buflen represents an overflow.
     */
    int snprintf(char * buf, size_t buflen, char const * format = nullptr) const;

    std::string to_string(char const * format = nullptr) const;

    /**
     * A canonical empty arg, useful as a constant.
     */
    static arg const & empty;

    /**
     * Construct an empty arg.
     */
    arg();

    enum value_type {
        vt_empty,
        vt_signed,
        vt_unsigned,
        vt_float,
        vt_date,
        vt_string,
        vt_cstr,
        vt_path,
        vt_str_view,
        vt_bool
    };
    value_type type;

    union {
        int64_t i64;
        uint64_t u64;
        double dbl;
        std::string const * str;
        char const * cstr;
        boost::filesystem::path const * path;
        str_view const * slice;
        bool boolean;
    };

    ~arg();

private:
    friend arg const & make_empty_arg();
};

}}} // end namespace

#include "core/text/arg-inline.h"

#endif // sentry
