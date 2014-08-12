#include <sstream>

#include "numeric_formats.h"

using std::string;
using std::stringstream;

std::string get_names_for_numeric_formats(numeric_formats f, char const * delim) {
    stringstream ss;
    unsigned n = 0;

    static_assert(static_cast<unsigned>(numeric_formats::decimal) == 1,
        "numeric_formats::decimal isn't lowest bit");
    static_assert(static_cast<unsigned>(numeric_formats::floating_point_only) << 1 >
                  static_cast<unsigned>(numeric_formats::all),
        "numeric_formats::floating_point isn't final value in enum");

    for (numeric_formats i = numeric_formats::decimal; i < numeric_formats::all;
        i = static_cast<numeric_formats>(static_cast<unsigned>(i) << 1)) {
        numeric_formats this_bit = f & i;
        if (static_cast<bool>(this_bit)) {
            if (n) {
                ss << delim;
            }
            switch (this_bit) {
            case numeric_formats::decimal: ss << "decimal"; break;
            case numeric_formats::octal: ss << "octal"; break;
            case numeric_formats::hex: ss << "hex"; break;
            case numeric_formats::binary: ss << "binary"; break;
            case numeric_formats::floating_point_only: ss << "float"; break;
            default: break;
            }
            ++n;
        }
    }
    return std::move(ss.str());
}

