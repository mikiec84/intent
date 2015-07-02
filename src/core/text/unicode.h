#ifndef _81ac2a416fd64b5f9321be3064496b31
#define _81ac2a416fd64b5f9321be3064496b31

#include <cstddef>
#include <cstdint>

namespace intent {
namespace core {
namespace text {

typedef uint32_t codepoint_t;
typedef uint16_t utf16_t;

const codepoint_t MAX_UNICODE_CHAR = 0x10FFFF;
const codepoint_t UNICODE_REPLACEMENT_CHAR = 0xFFFD;

// Yes, the HIGH_SURROGATE_* constants have smaller values than the
// LOW_SURROGATE_* ones. This is not a bug. "High" and "Low" refer to the
// portions of the 10-bit codepoint they encode, not to the surrogate
// values themselves.
const codepoint_t HIGH_SURROGATE_BEGIN = 0xD800;
const codepoint_t HIGH_SURROGATE_END = 0xDBFF;
const codepoint_t LOW_SURROGATE_BEGIN = 0xDC00;
const codepoint_t LOW_SURROGATE_END = 0xDFFF;

/**
 * @return true if char is a valid utf8 lead byte. Disallows bytes used in
 *     overlong encodings (0xC0 and 0xC1) as well as bytes used in theoretical
 *     but disallowed 5- and 6-byte sequences (0xF5 to 0xFF).
 */
bool is_utf8_lead_byte(char c);

bool is_utf8_lead_byte(uint8_t c);

bool is_utf8_continuation_byte(char c);

bool is_utf8_continuation_byte(uint8_t c);

bool is_surrogate(codepoint_t);

bool is_low_surrogate(codepoint_t);

bool is_high_surrogate(codepoint_t);

codepoint_t decode_surrogate(codepoint_t high, codepoint_t low);

codepoint_t decode_surrogate(utf16_t high, utf16_t low);

/**
 * @return How many bytes are used to encode the codepoint at the beginning of
 *     the buffer, or 0 if text is malformed (in which case, an invalid byte
 *     sequence exists; pointer should advance by one byte only, and restart
 *     analysis). Overlong encodings are not allowed (causing 0 to be returned);
 *     however, surrogates are treated as valid (allowing CESU-8 semantics if
 *     desired).
 */
size_t length_of_codepoint(char const * txt);


/**
 * @return How many bytes should be used to encode a particular codepoint. Note
 *     that surrogates are treated as standalone codepoints.
 */
size_t proper_length_of_codepoint(codepoint_t);


/**
 * @return How many bytes should be used to encode a codepoint, given a particular
 *     leady byte. This length may not prove accurate if trail bytes are
 *     malformed, so use this function only when prepared to test for
 *     wellformedness. The null char (char 0) yields 0.
 */
size_t predict_length_of_codepoint_from_lead_byte(char lead_byte);


/**
 * @return true if buffer obeys all rules of utf8 formation. Returns false if
 *     txt is null. Does not allow overlong encodings.
 */
bool is_well_formed_utf8(char const * txt);


/**
 * Decodes one unicode codepoint in the utf8 buffer, and advances pointer
 * beyond its boundary. This function validates lead byte and trail byte values
 * in cheap ways (e.g., disallowing 5- and 6-byte sequences; disallowing
 * illegal lead bytes). However, some forms of invalid utf8 can still leak
 * through, such as overlong encodings and 4-byte sequences encoding codepoints
 * above 0x10FFFF. This is a feature, not a bug; besides improving performance,
 * it allows you to build support for utf8 variants such as CESU-8 (see
 * http://j.mp/1HzJPBY) and Modified UTF-8 (see http://j.mp/1GRqXbX).
 *
 * @param txt utf8
 * @param cp Receives the codepoint at the current location; set to
 *     UNICODE_REPLACEMENT_CHAR on error. (Note that UNICODE_REPLACEMENT_CHAR
 *     can also be encoded deliberately in valid utf8, so this value is only
 *     an indication of an error if the return value is 1 byte beyond txt
 *     instead of the 2 bytes returned by the non-error case.)
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
 * Does not null-terminate. Note that this may create utf8 that is technically
 * invalid per the spec, if cp is a surrogate. This is a feature, not a bug,
 * because it allows you to build support for CESU-8 if you like. To avoid
 * the surrogate issue, convert a whole buffer of codepoint_t values or utf16_t
 * values (see {@link #convert_to_utf8}) so surrogate pairs can be processed
 * as a unit.
 *
 * @param buf
 *     The buffer into which the sequence is written. Advanced if anything is
 *     written.
 * @param buf_length
 *     Number of bytes available in buf. Advanced if anything is written.
 * @return true if anything was written
 */
bool add_codepoint_to_utf8(char *& buf, size_t & buf_length, codepoint_t cp);

void convert_to_utf8(char *& buf, size_t & buf_length, codepoint_t const * txt);
void convert_to_utf8(char *& buf, size_t & buf_length, utf16_t const * txt);

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

}}} // end namespace

#include "core/text/unicode-inline.h"

#endif // sentry
