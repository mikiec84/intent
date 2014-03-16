#ifndef UNICODEINLINE_H_c3579b98cf454bf5a90ffa0c7394f6ec
#define UNICODEINLINE_H_c3579b98cf454bf5a90ffa0c7394f6ec

#ifndef UNICODE_H_a2c4bf80275c4eebb036e7a63c2ed4ca
#include "core/unicode.h"
#endif

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

inline bool cat_codepoint_to_utf8(char *& buf, size_t & buf_length, codepoint_t cp) {
    bool ok = add_codepoint_to_utf8(buf, buf_length, cp);
    if (ok && buf_length > 0) {
        *buf = 0;
    }
    return ok;
}

inline bool cat_unicode_escape_sequence(char *& buf, size_t & buf_length, codepoint_t cp) {
    bool ok = add_unicode_escape_sequence(buf, buf_length, cp);
    if (ok && buf_length > 0) {
        *buf = 0;
    }
    return ok;
}

#endif // sentry
