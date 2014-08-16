#ifndef _scan_numbers_h_d93b806d9f6d425c967c1228eb430ef8
#define _scan_numbers_h_d93b806d9f6d425c967c1228eb430ef8

#include <cstdint>

#include "numeric_formats.h"

typedef char const * (* scan_digits_func)(char const * p, char const * end, uint64_t & n);

/**
 * Read digits until end of hexadecimal number. Put numeric value into n.
 * @return offset of first char beyond digits of the number.
 */
char const * scan_hex_digits(char const * p, char const * end, uint64_t & n);

/**
 * Read digits until end of binary number. Put numeric value into n.
 * @return offset of first char beyond digits of the number.
 */
char const * scan_binary_digits(char const * p, char const * end, uint64_t & n);

/**
 * Read digits until end of octal number. Put numeric value into n.
 * @return offset of first char beyond digits of the number.
 */
char const * scan_octal_digits(char const * p, char const * end, uint64_t & n);

/**
 * Read digits before a radix, until end of decimal number. Put numeric value into n.
 * @return offset of first char beyond digits of the number.
 */
char const * scan_decimal_digits_pre_radix(char const * p, char const * end, uint64_t & n);

/**
 * Read digits after a radix, until end of decimal number. Put numeric value (always < 1)
 * into n.
 * @return offset of first char beyond digits of the number.
 */
char const * scan_decimal_digits_post_radix(char const * p, char const * end, double & n);

/**
 * Read a number into n, handling a possible preceding sign.
 * @return offset of first char beyond digits of the number.
 */
char const * scan_decimal_number(char const * p, char const * end, bool & negative, uint64_t & n);

/**
 * Describes a numeric literal found in text.
 */
struct number_info {
    numeric_formats format;
    union {
        double floating_point;
        uint64_t whole_number;
    };
    bool negative;
};

/**
 * Read a number from text. Return ptr to first char beyond number on success,
 * or unmoved pointer on failure.
 *
 * This scanning function is very flexible; it is the same one used by the
 * compiler to interpret numeric literals. Format is automatically detected;
 * decimal, hex, octal, and binary whole numbers, as well as floating point,
 * are all supported. Numbers can also have _ delimiters for grouping. Valid
 * values include things like:
 *
 *   3.14
 *   0x409 (note that hex must have the 0x or 0X prefix)
 *   0777 (octal is detected by leading 0)
 *   0b1001_0001
 *   -293_842_392_394_982
 *   6.023e27
 *
 * @param txt: Text to scan
 * @param end: First char beyond end of text; if nullptr, the null char at
 *     end of txt is assumed.
 * @param numeric_formats: Specifies which formats should be allowed. If a
 *     disallowed format is detected, the pointer is not advanced, but
 *     number_info is fully populated.
 * @param number_info: OUT. Holds info about numeric value. Read from
 *     floating_point if formats == numeric_formats::floating_point, or from
 *     whole_number otherwise.
 */
char const * scan_number(char const * txt, char const * end, numeric_formats
    formats, number_info & info);

#endif // sentry
