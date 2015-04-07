#ifndef _70b64dcdd0ac44a1af96e5dcc029c337
#define _70b64dcdd0ac44a1af96e5dcc029c337

#include <cstdint>

namespace intent {
namespace core {
namespace text {

struct unicode_range_info {
    uint32_t begin;
    uint32_t end;
    char const * name;
    char const * used_by_langs;
};

unicode_range_info const * get_unicode_range_info(uint32_t codepoint);

}}} // end namespace

#endif // sentry
