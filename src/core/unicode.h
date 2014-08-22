#ifndef intent_core_unicode_h
#define intent_core_unicode_h

#include <cstddef>
#include <cstdint>

namespace intent {
namespace core {

typedef uint32_t codepoint_t;

const codepoint_t MAX_UNICODE_CHAR = 0x10FFFF;
const codepoint_t UNICODE_REPLACEMENT_CHAR = 0xFFFD;

bool is_utf8_lead_byte(char c);
bool is_utf8_lead_byte(uint8_t c);

bool is_utf8_trail_byte(char c);
bool is_utf8_trail_byte(uint8_t c);

/**
 * @return Theoretical length of unicode char at current offset. This length
 *     may not be accurate if unicode is malformed. Call {@link
 *     get_codepoint_from_utf8(char const *)} to discover actual length.
 */
inline size_t length_of_codepoint(char const * txt);

/**
 * Advance across one unicode codepoint in the utf8 buffer.
 *
 * @param txt utf8
 * @param cp Receives the codepoint at the current location; set to
 *     UNICODE_REPLACEMENT_CHAR on error.
 * @return Location of the beginning of the next codepoint in the string.
 */
char const * get_codepoint_from_utf8(char const * txt, codepoint_t & cp);

/**
 * Advance to the beginning of the next logical character.
 *
 * Since this function is the analog of a simple increment on a char const * and
 * will be used in very tight loops, this function does no error checking.
 * @return offset of beginning of next logical utf8 character, or the null
 *     terminator if txt is pointing to the final valid character when the
 *     function is called. This lets you loop like this:
 *         for (char const * p = input; *p; p = next_utf8_char(p))
 */
char const * next_utf8_char_inline(char const * txt);

/**
 * Same as next_utf8_char_inline(), except implemented as a true function. This
 * allows it to be used as a function pointer in some algorithms.
 */
char const * next_utf8_char(char const * txt);

/**
 * Append the appropriate utf-8 byte sequence to represent a single codepoint.
 * Does not null-terminate.
 *
 * @param buf
 *     The buffer into which the sequence is written. Advanced if anything is
 *     written.
 * @param buf_length
 *     Number of bytes available in buf. Advanced if anything is written.
 * @return true if anything was written
 */
bool add_codepoint_to_utf8(char *& buf, size_t & buf_length, codepoint_t cp);

/**
 * Same as add_codepoint_to_utf8, but null-terminates (unless there's no room
 * to do so).
 */
bool cat_codepoint_to_utf8(char *& buf, size_t & buf_length, codepoint_t cp);

/**
 * Like strlen(), except counts codepoints rather than bytes.
 * @param utf8 The string to scan.
 * @return Number of codepoints found.
 */
size_t count_codepoints_in_utf8(char const * utf8, char const * end = 0);

/**
 * In a string buffer, read an escape sequence and return the cp
 * that it represents. Supports:
 *
 *   traditional C-style escape sequences (\r, \n, \t, \b, \f, \v, \a)
 *   2 hex chars, such as \xC7
 *   3 octal chars, such as \023
 *   ucs2 codepoints, such as \u2029
 *   ucs4 codepoints, such as \U0010AE4F
 *
 * If a sequence is invalid, cp is set to UNICODE_REPLACEMENT_CHAR.
 */
char const * scan_unicode_escape_sequence(char const * seq, codepoint_t & cp);

/**
 * Append the correct escape sequence for a particular unicode codepoint.
 * Will append raw ascii for characters that do not need to be escaped.
 * Does not null-terminate.
 */
bool add_utf8_or_escape_sequence(char *& buf, size_t & buf_length, codepoint_t cp);

/**
 * Same as add_utf8_or_escape_sequence(), only null-terminates.
 */
bool cat_utf8_or_escape_sequence(char *& buf, size_t & buf_length, codepoint_t cp);

/**
 * Analogous to strchr(), except searches for any unicode codepoint.
 * @param utf8 String to search.
 * @param cp Codepoint to search for.
 * @return offset of cp in buffer, or nullptr on failure.
 */
char const * find_codepoint_in_utf8(char const * utf8, codepoint_t cp);

/**
 * Analogous to strchr(), except searches for any unicode codepoint.
 * @param utf8 String to search.
 * @param end First char beyond search range.
 * @param cp Codepoint to search for.
 * @return offset of cp in buffer, or nullptr on failure.
 */
char const * find_codepoint_in_utf8(char const * utf8, char const * end,
        codepoint_t cp);

}} // end namespace

#include "core/unicode-inline.h"

#endif // sentry
