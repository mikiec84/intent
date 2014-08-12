#include <cmath>
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
    n = 0;
    negative = false;
    if (p && p < end) {
        char c = *p;
        negative = (c == '-');
        if (c == '+' || c == '-') {
            ++p;
        }
        p = scan_decimal_digits_pre_radix(p, end, n);
    }
    return p;
}

char const * scan_number(char const * text, char const * end, numeric_formats
    allowed_formats, number_info & info) {
    char const * p = text;

    info.whole_number = 0;

    char c = *p;

    info.negative = (c == '-');
    if (c == '+' || c == '-') {
        c = *++p;
    }

    bool floating_point = (c == '.');

    if (!floating_point) {
        if (c == '0') {
            if (p + 2 < end) {
                c = p[1];
                info.format = numeric_formats::all;
                if (c == 'x' || c == 'X') {
                    p = scan_hex_digits(p + 2, end, info.whole_number);
                    info.format = numeric_formats::hex;
                } else if (c == 'b' || c == 'B') {
                    p = scan_binary_digits(p + 2, end, info.whole_number);
                    info.format = numeric_formats::binary;
                } else if (c >= '0' && c <= '7') {
                    p = scan_octal_digits(p + 2, end, info.whole_number);
                    info.format = numeric_formats::octal;
                }
                if (info.format != numeric_formats::all) {
                    return (static_cast<bool>(info.format & allowed_formats)) ? p : text;
                }
            }
        }
        p = scan_decimal_digits_pre_radix(p, end, info.whole_number);
    }

    double significand, value;

    if (!floating_point && p < end && *p == '.') {
        floating_point = true;
        p = scan_decimal_digits_post_radix(++p, end, significand);
    } else {
        significand = 0.0;
    }

    // Now we've read everything except possibly an exponent. Make sure we've
    // combined values to left and right of radix, if appropriate.
    if (floating_point) {
        significand += info.whole_number;
    }

    // Check for exponent.
    if (p + 1 < end && (*p == 'e' || *p == 'E')) {
        floating_point = true;

        bool negative_exponent;
        uint64_t exponent;
        p = scan_decimal_number(++p, end, negative_exponent, exponent);
        double exp = exponent;
        if (negative_exponent) {
            exp *= -1;
        }
        // TODO: what if exponent is too big? What if there's nothing after "e"?
        value = significand * pow(10, exp);
        if (info.negative) {
            value *= -1;
        }
    } else if (floating_point) {
        value = significand;
    }

    if (floating_point) {
        info.format = numeric_formats::floating_point_only;
        info.floating_point = value;
    } else {
        info.format = numeric_formats::decimal;
    }
    return (static_cast<bool>(info.format & allowed_formats)) ? p : text;
}



