#ifndef SCAN_NUMBERS_H_d93b806d9f6d425c967c1228eb430ef8
#define SCAN_NUMBERS_H_d93b806d9f6d425c967c1228eb430ef8

#include <cstdint>

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

#endif // sentry
