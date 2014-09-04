#ifndef intent_core_unicode_inline_h
#define intent_core_unicode_inline_h

#include "core/unicode.h"

namespace intent {
namespace core {
namespace text {

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

inline char const * next_utf8_char_inline(char const * txt) {
    uint8_t c;
    do {
        c = static_cast<uint8_t>(*++txt);
    } while (c >= 0x80 && !is_utf8_lead_byte(c));
    return txt;
}

}}} // end namespace

#endif // sentry
