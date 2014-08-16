#ifndef _unicode_inline_h_c3579b98cf454bf5a90ffa0c7394f6ec
#define _unicode_inline_h_c3579b98cf454bf5a90ffa0c7394f6ec

#include "core/unicode.h"

namespace intent {
namespace core {

inline bool is_utf8_trail_byte(uint8_t c) {
    // Top bit is set, but not next bit
    return (static_cast<uint8_t>(c) & 0xC0) == 0x80;
}

inline bool is_utf8_trail_byte(char c) {
    return is_utf8_trail_byte(static_cast<uint8_t>(c));
}

inline bool is_utf8_lead_byte(uint8_t c) {
    // Top 2 bits are set
    return (static_cast<uint8_t>(c) & 0xC0) == 0xC0;
}

inline bool is_utf8_lead_byte(char c) {
    return is_utf8_lead_byte(static_cast<uint8_t>(c));
}

}} // end namespace

#endif // sentry
