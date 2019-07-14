#ifndef _daf9cd99359141a8a414f60344e3f78d
#define _daf9cd99359141a8a414f60344e3f78d

#include "core/text/unicode.h"

namespace intent {
namespace core {
namespace text {


inline bool is_utf8_continuation_byte(uint8_t c) {
	// Top bit is set, but not next bit
	return (static_cast<uint8_t>(c) & 0xC0) == 0x80;
}


inline bool is_utf8_continuation_byte(char c) {
	return is_utf8_continuation_byte(static_cast<uint8_t>(c));
}


inline bool is_utf8_lead_byte(uint8_t c) {
	return c >= 0xC2 && c <= 0xF4;
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


inline size_t predict_length_of_codepoint_from_lead_byte(char ch) {
	auto c = static_cast<uint8_t>(ch);
	if (c < 0x80) {
		return c ? 1 : 0;
	} else if ((c & 0xE0) == 0xC0) { /* 110X XXXX  2 bytes */
		return c >= 0xC2 ? 2 : 0;
	} else if ((c & 0xF0) == 0xE0) { /* 1110 XXXX  3 bytes */
		return 3;
	} else if ((c & 0xF0) == 0xF0 && c <= 0xF4) { /* 1111 0XXX  4 bytes */
		return 4;
	}
	// Could be an invalid lead byte, or a trail byte.
	return 0;
}


inline size_t proper_length_of_codepoint(codepoint_t cp) {
	if (cp <= 0x007F) {
		return 1;
	} else if (cp <= 0x07FF) {
		return 2;
	} else if (cp <= 0xFFFF) {
		return 3;
	} else {
		return 4;
	}
}


inline bool is_surrogate(codepoint_t cp) {
	return cp >= HIGH_SURROGATE_BEGIN && cp <= LOW_SURROGATE_END;
}


inline bool is_low_surrogate(codepoint_t cp) {
	return cp >= LOW_SURROGATE_BEGIN && cp <= LOW_SURROGATE_END;
}


inline bool is_high_surrogate(codepoint_t cp) {
	return cp >= HIGH_SURROGATE_BEGIN && cp <= HIGH_SURROGATE_END;
}


inline size_t length_of_codepoint(char const * txt) {
	if (txt == nullptr) {
		return 0;
	}
	// How many bytes do we need to test for validity?
	switch (predict_length_of_codepoint_from_lead_byte(*txt)) {
	case 1:
		return 1;
	case 2:
		return is_utf8_continuation_byte(txt[1]) ? 2: 0;
	case 3:
		return is_utf8_continuation_byte(txt[1]) && is_utf8_continuation_byte(txt[2]) ? 3 : 0;
	case 4:
		if (!is_utf8_continuation_byte(txt[1]) || !is_utf8_continuation_byte(txt[2]) || !is_utf8_continuation_byte(txt[3])) {
			return 0;
		}
		// Some 4-byte encodings are invalid because they exceed 0x10FFFF or
		// because they encode something that could have been encoded with
		// less bytes...
		codepoint_t cp;
		get_codepoint_from_utf8(txt, cp);
		if (cp > MAX_UNICODE_CHAR) {
			return 0;
		}
		return 4;
	default:
		// RFC 3629 restricted utf8 to end at U+10FFFF, which makes 4-byte
		// sequences the upper end of the valid range.
		return 0;
	}
}


}}} // end namespace

#endif // sentry
