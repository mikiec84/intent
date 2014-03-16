// Derived from public domain code by Jeff Bezanson; see
// http://www.cprogramming.com/tutorial/seq.c.

#include <cstring>
#include <cstdio>
#include <cstdlib>

#include "core/scan_numbers.h"
#include "core/unicode.h"

inline size_t length_of_codepoint_from_char(char ch) {
    unsigned c = static_cast<unsigned>(ch);
    if (c < 0x80) {
        return 1;
    } else if ((c & 0xE0) == 0xC0) { /* 110X XXXX  2 bytes */
        return 2;
    } else if ((c & 0xF0) == 0xE0) { /* 1110 XXXX  3 bytes */
        return 3;
    } else if ((c & 0xF8) == 0xF0) { /* 1111 0XXX  4 bytes */
        return 4;
    }
    // Could be an invalid lead byte, or a trail byte.
    return 0;
}

inline size_t length_of_codepoint(char const * utf8) {
    if (utf8 == nullptr) return 0;
    return length_of_codepoint_from_char(*utf8);
}

char const * get_codepoint_from_utf8(char const * utf8, codepoint_t & cp)
{
    if (utf8 == nullptr) return nullptr;
    char c = *utf8++;
    size_t byte_count = length_of_codepoint_from_char(c);

    #define or_trail_byte() \
        cp <<= 6; \
        c = *utf8++; \
        if (!is_utf8_trail_byte(c)) { \
            cp = UNICODE_REPLACEMENT_CHAR; \
            return utf8 - 1; \
        } \
        cp |= static_cast<codepoint_t>(c & 0x3F)

    switch (byte_count) {
    case 1:
        cp = static_cast<codepoint_t>(c);
        break;
    case 2:
        cp = static_cast<codepoint_t>(c & 0x1F);
        or_trail_byte();
        break;
    case 3:
        cp = static_cast<codepoint_t>(c & 0x0F);
        or_trail_byte();
        or_trail_byte();
        break;
    case 4:
        cp = static_cast<codepoint_t>(c & 0x07);
        or_trail_byte();
        or_trail_byte();
        or_trail_byte();
        break;
    default:
        cp = UNICODE_REPLACEMENT_CHAR;
        break;
    }
    return utf8;
}

/**
 * Append the appropriate utf-8 byte sequence to represent a single cp
 * from unicode. Does not null-terminate.
 *
 * @param buf
 *     The buffer into which the sequence is written.
 * @param buf_length
 *     Number of bytes available in buf.
 * @return offset of first byte beyond written position. If write failed
 *     due to lack of space, return buf.
 */
bool add_codepoint_to_utf8(char *& buf, size_t & buf_length, codepoint_t cp)
{
    if (buf == nullptr || buf_length < 1) {
        return false;
    }

    if (cp < 0x80) {
        *buf++ = (char)cp;

    } else if (cp < 0x800) {
        if (buf_length < 2)
            return false;
        *buf++ = (cp>>6) | 0xC0;
        *buf++ = (cp & 0x3F) | 0x80;

    } else if (cp < 0x10000) {
        if (buf_length < 3)
            return false;
        *buf++ = (cp>>12) | 0xE0;
        *buf++ = ((cp>>6) & 0x3F) | 0x80;
        *buf++ = (cp & 0x3F) | 0x80;

    } else if (cp < 0x110000) {
        if (buf_length < 4)
            return false;
        *buf++ = (cp>>18) | 0xF0;
        *buf++ = ((cp>>12) & 0x3F) | 0x80;
        *buf++ = ((cp>>6) & 0x3F) | 0x80;
        *buf++ = (cp & 0x3F) | 0x80;
    }
    return true;
}

size_t count_codepoints_in_utf8(char const * utf8, char const * end)
{
    size_t count = 0;
    codepoint_t cp;
    while (utf8 < end) {
        utf8 = get_codepoint_from_utf8(utf8, cp);
        ++count;
    }
    return count;
}

const char * const NEED_1CHAR_ESCAPES = "\n\r\t\f\v\b\a";
const char * const CHARS_FOR_1CHAR_ESCAPES = "nrtfvba";

/**
 * In a string buffer, read an escape sequence, beginning at the char after
 * the backslash, and return the cp that it represents. Supports:
 *
 *   traditional C-style escape sequences (\r, \n, \t, \b, \f, \v, \a)
 *   2 hex chars, such as \xC7
 *   3 octal chars, such as \023
 *   ucs2 codepoints, such as \u2029
 *   ucs4 codepoints, such as \U0010AE4F
 *
 * If a sequence is invalid, cp is set to UNICODE_REPLACEMENT_CHAR.
 */
char const * scan_unicode_escape_sequence(char const * seq, codepoint_t & cp)
{
    if (seq == nullptr) return nullptr;
    scan_digits_func scan_func = scan_hex_digits;
    char const * end = seq + 1;
    char c = *seq++;
    switch (c) {
    case 'x':
        end = seq + 2; break;
    case 'u':
        end = seq + 4; break;
    case 'U':
        end = seq + 8; break;
    case '\\':
        cp = '\\'; return end;
    default:
        if (c >= '0' && c <= '7') {
            seq -= 1;
            end = seq + 3;
            scan_func = scan_octal_digits;
        } else {
            if (c) {
                char const * p = strchr(CHARS_FOR_1CHAR_ESCAPES, c);
                if (p) {
                    cp = NEED_1CHAR_ESCAPES[p - CHARS_FOR_1CHAR_ESCAPES];
                    return end;
                }
            }
            cp = UNICODE_REPLACEMENT_CHAR;
            return seq;
        }
        break;
    }

    uint64_t n;
    char const * p = scan_func(seq, end, n);
    if (p == end) {
        // Check for overflow on ucs4
        if (end == seq + 8) {
            if (n > MAX_UNICODE_CHAR) {
                cp = UNICODE_REPLACEMENT_CHAR;
                return seq;
            }
        }
        cp = static_cast<codepoint_t>(n);
        return p;
    }
    cp = UNICODE_REPLACEMENT_CHAR;
    return seq;
}

#define advance_and_return_true(n) buf += n; buf_length += n; return true

inline bool write_2byte_sequence(char *& buf, size_t & buf_length, char c) {
    if (buf_length >= 2) {
        buf[0] = '\\';
        buf[1] = c;
        advance_and_return_true(2);
    }
    return false;
}

bool add_unicode_escape_sequence(char *& buf, size_t & buf_length, codepoint_t cp) {
    if (buf == nullptr || buf_length < 1) {
        return false;
    }
    if (cp < 0x80) {
        if (cp < ' ') {
            if (cp != 0) {
                const char * p = strchr(NEED_1CHAR_ESCAPES, static_cast<char>(cp));
                if (p) {
                    return write_2byte_sequence(buf, buf_length, CHARS_FOR_1CHAR_ESCAPES[p - NEED_1CHAR_ESCAPES]);
                }
            }
            if (buf_length > 4) {
                snprintf(buf, buf_length, "\\x%.2X", static_cast<uint8_t>(cp));
                advance_and_return_true(4);
            }
            return false;
        } else if (cp == '\\') {
            return write_2byte_sequence(buf, buf_length, '\\');
        } else if (cp == 0x7f) {
            if (buf_length >= 4) {
                memcpy(buf, "\\x7F", 4);
                advance_and_return_true(4);
            }
            return false;
        }
        // No escape sequence needed.
        *buf = static_cast<uint8_t>(cp);
        advance_and_return_true(1);
    } else if (cp <= 0xFFFF) {
        if (buf_length >= 6) {
            snprintf(buf, buf_length, "\\u%.4hX", static_cast<uint16_t>(cp));
            advance_and_return_true(6);
        }
        return false;
    }
    if (buf_length >= 10) {
        snprintf(buf, buf_length, "\\U%.8X", cp);
        advance_and_return_true(10);
    }
    return false;
}

char const * find_codepoint_in_utf8(char const * utf8, codepoint_t cp) {
    if (utf8 == nullptr) {
        return nullptr;
    }
    while (true) {
        if (*utf8 == 0) {
            return nullptr;
        }
        codepoint_t found;
        char const * p = get_codepoint_from_utf8(utf8, found);
        if (cp == found) {
            return utf8;
        }
        utf8 = p;
    }
}

char const * find_codepoint_in_utf8(char const * utf8, char const * end, codepoint_t cp) {
    if (utf8 == nullptr) {
        return nullptr;
    }
    while (true) {
        if (utf8 >= end) {
            return nullptr;
        }
        codepoint_t found;
        char const * p = get_codepoint_from_utf8(utf8, found);
        if (cp == found) {
            return utf8;
        }
        utf8 = p;
    }
}
