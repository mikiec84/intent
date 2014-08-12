#ifndef NUMERIC_FORMATS_H
#define NUMERIC_FORMATS_H

#include <string>
#include "enum_operators.h"

/**
 * Describe numeric formats that might be allowed for a particular token. This
 * enum supports bitmasking.
 */
enum class numeric_formats : unsigned {
    decimal = 1,
    octal = 2,
    hex = 4,
    binary = 8,
    floating_point_only = 16,
    floating_point = 16 | 1,
    all = 16 | 8 | 4 | 2 | 1,
};
define_bitwise_operators_for_enum(numeric_formats);

/**
 * Provide a way to convert a bitmask to text.
 */
std::string get_names_for_numeric_formats(numeric_formats,
    char const * delim = "|");

#endif // NUMERIC_FORMATS_H
