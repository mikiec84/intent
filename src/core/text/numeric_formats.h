#ifndef _c0a5080fd31546cf995b2c4716894409
#define _c0a5080fd31546cf995b2c4716894409

#include <string>
#include "core/util/enum_operators.h"

namespace intent {
namespace core {
namespace text {

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

/**
 * Provide a way to convert a bitmask to text.
 */
std::string get_names_for_numeric_formats(numeric_formats,
    char const * delim = "|");

}}} // end namespace

define_bitwise_operators_for_enum(intent::core::text::numeric_formats);

#endif // sentry
