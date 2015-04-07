#include <algorithm>

#include "core/text/unicode_range.h"
#include "core/util/countof.h"


namespace intent {
namespace core {
namespace text {


static constexpr unicode_range_info const range_infos[] = {
    #define tuple(begin_range, end_range, range_name, lang_codes) \
        { begin_range, end_range, range_name, lang_codes },
    #include "core/text/unicode_range.tuples"
};

static constexpr unsigned range_infos_count = countof(range_infos);
static constexpr unicode_range_info const * range_infos_begin = &range_infos[0];
static constexpr unicode_range_info const * range_infos_end = &range_infos[range_infos_count];


static bool range_is_less(unicode_range_info const & a, unicode_range_info const & b) {
    return a.end <= b.end;
}


unicode_range_info const * get_unicode_range_info(uint32_t codepoint) {
    if (codepoint < 0x110000) {
        unicode_range_info x;
        x.end = codepoint;
        auto first_not_lower = std::lower_bound(range_infos_begin, range_infos_end,
                x, range_is_less);
        if (first_not_lower != range_infos_end) {
            if (first_not_lower->begin <= codepoint) {
                return first_not_lower;
            }
        }
    }
    return nullptr;
}


}}} // end namespace
