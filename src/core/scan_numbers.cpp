#include "core/scan_numbers.h"

/**
 * Read digits until end of hexadecimal number. Put numeric value into n.
 * @return offset of first char beyond digits of the number.
 */
char const * scan_hex_digits(char const * p, char const * end, uint64_t & n) {
    n = 0;
    while (p < end) {
        char c = *p;
        if (c >= '0' && c <= '9') {
            n = (n * 16) + (c - '0');
        } else if (c >= 'a' && c <= 'f') {
            n = (n * 16) + 10 + (c - 'a');
        } else if (c >= 'A' && c <= 'F') {
            n = (n * 16) + 10 + (c - 'A');
        } else if (c != '_') {
            break;
        }
        ++p;
    }
    return p;
}

/**
 * Read digits until end of binary number. Put numeric value into n.
 * @return offset of first char beyond digits of the number.
 */
char const * scan_binary_digits(char const * p, char const * end, uint64_t & n) {
    n = 0;
    while (p < end) {
        char c = *p;
        if (c == '0') {
            n <<= 1;
        } else if (c == '1') {
            n = (n << 1) | 1;
        } else if (c != '_') {
            break;
        }
        ++p;
    }
    return p;
}

/**
 * Read digits until end of octal number. Put numeric value into n.
 * @return offset of first char beyond digits of the number.
 */
char const * scan_octal_digits(char const * p, char const * end, uint64_t & n) {
    n = 0;
    while (p < end) {
        char c = *p;
        if (c >= '0' && c <= '7') {
            n = (n * 8) + (c - '0');
        } else if (c != '_') {
            break;
        }
        ++p;
    }
    return p;
}

/**
 * Read digits before a radix, until end of decimal number. Put numeric value into n.
 * @return offset of first char beyond digits of the number.
 */
char const * scan_decimal_digits_pre_radix(char const * p, char const * end, uint64_t & n) {
    n = 0;
    while (p < end) {
        char c = *p;
        if (c >= '0' && c <= '9') {
            n = (n * 10) + (c - '0');
        } else if (c != '_') {
            break;
        }
        ++p;
    }
    return p;
}

/**
 * Read digits after a radix, until end of decimal number. Put numeric value into n.
 * @return offset of first char beyond digits of the number.
 */
char const * scan_decimal_digits_post_radix(char const * p, char const * end, double & n) {
    n = 0.0;
    double divisor = 10.0;
    while (p < end) {
        char c = *p;
        if (c >= '0' && c <= '9') {
            n += ((c - '0') / divisor);
            divisor *= 10;
        } else if (c != '_') {
            break;
        }
        ++p;
    }
    return p;
}

char const * scan_decimal_number(char const * p, char const * end, bool & negative, uint64_t & n) {
    if (p < end) {
        char c = *p;
        negative = (c == '-');
        if (c == '+' || c == '-') {
            ++p;
        }
        p = scan_decimal_digits_pre_radix(p, end, n);
    }
    return p;
}

