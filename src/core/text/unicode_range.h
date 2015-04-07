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


/**
 * Look up information about the range of unicode characters to which a
 * particular codepoint belongs.
 * @param codepoint
 * @return null if character is outside valid unicode range [0..0x110000), or
 *     if the codepoint is a control character with no other defined semantics.
 *     Otherwise, a structure that identifies the range the character belongs
 *     to.
 */
unicode_range_info const * get_unicode_range_info(uint32_t codepoint);

}}} // end namespace

#endif // sentry
