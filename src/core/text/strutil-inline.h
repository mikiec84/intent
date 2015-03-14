#ifndef _57fb304ec4644128a5111116d6bfd9ff
#define _57fb304ec4644128a5111116d6bfd9ff

#include <cstring>

#include "core/text/strutil.h"

namespace intent {
namespace core {
namespace text {

inline char const * find_char(char const * p, char c, char const * end) {
	if (p == nullptr) return nullptr;
	if (end < p) return p;
	for (; p < end; ++p) {
		if (*p == c) {
			return p;
		}
	}
	return end;
}

inline char const * find_char(char const * p, char c, str_range range) {
	return find_char(p + range.begin, c, p + range.end);
}

inline char const * find_any_char(char const * p, char const * any, char const * end) {
	if (p == nullptr) return nullptr;
	if (end < p) return p;
	for (; p < end; ++p) {
		if (strchr(any, *p)) {
			return p;
		}
	}
	return end;
}

inline char const * find_any_char(char const * p, char const * any, str_range range) {
	return find_any_char(p + range.begin, any, p + range.end);
}

inline char const * ltrim(char const * begin, char const * end, char const * chars) {
	if (begin == nullptr) return nullptr;
	if (end < begin) return begin;
	if (chars == nullptr) chars = ANY_WHITESPACE;
	while (begin < end) {
		if (strchr(chars, *begin) == nullptr) {
			return begin;
		}
		++begin;
	}
	return begin;
}

inline char const * rtrim(char const * begin, char const * end, char const * chars) {
	if (begin == nullptr) return nullptr;
	if (end < begin) return begin;
	if (chars == nullptr) chars = ANY_WHITESPACE;
	while (--end >= begin) {
		if (strchr(chars, *end) == nullptr) {
			return end + 1;
		}
	}
	return begin;
}

inline bool is_null_or_empty(char const * p) {
	return p == nullptr || *p == 0;
}

inline char const * scan_spaces_and_tabs(char const * p, char const * end) {
	while (p < end) {
		char c = *p;
		if (c != ' ' && c != '\t') {
			break;
		}
		++p;
	}
	return p;
}

inline char const * consume_line_break(char const * p, char const * end) {
	if (p + 1 < end) {
		if (*p == '\r') {
			if (p[1] == '\n') {
				++p;
			}
		}
	}
	++p;
	return p;
}

inline char * ascii_to_lower_case(char * ptr) {
	if (ptr) {
		char * p = ptr;
		while (*p) {
			char c = *p;
			if (c <= 'Z' && c >= 'A') {
				*p += 32;
			}
			++p;
		}
	}
	return ptr;
}

inline char * ascii_to_lower_case(char * ptr, char const * end) {
	if (ptr) {
		char * p = ptr;
		while (p < end) {
			char c = *p;
			if (c <= 'Z' && c >= 'A') {
				*p += 32;
			}
			++p;
		}
	}
	return ptr;
}


inline char * ascii_to_upper_case(char * ptr) {
	if (ptr) {
		char * p = ptr;
		while (*p) {
			char c = *p;
			if (c >= 'a' && c <= 'z') {
				*p -= 32;
			}
			++p;
		}
	}
	return ptr;
}


inline char * ascii_to_upper_case(char * ptr, char const * end) {
	if (ptr) {
		char * p = ptr;
		while (p < end) {
			char c = *p;
			if (c >= 'a' && c <= 'z') {
				*p -= 32;
			}
			++p;
		}
	}
	return ptr;
}


inline int compare_str_ascii_case_insensitive(char const * a, char const * b) {
	if (a) {
		if (b) {
			while (true) {
				char cha = *a;
				char chb = *b;
				int n = (cha - chb);
				if (n) {
					// These conditionals are carefully structured to be as
					// efficient as possible. They eliminate the biggest possible
					// ranges with the least number of tests. We normalize to
					// upper case rather than lower case because we want to
					// treat all alphas as if they appear where the upper case
					// chars appear in the ASCII range--not where the lower case
					// chars appear.
					if (cha >= 'a' && chb <= 'Z' && chb >= 'A' && cha <= 'z') {
						cha -= 32;
						n = (cha - chb);
					} else if (chb >= 'a' && cha <= 'Z' && cha >= 'A' && chb <= 'z') {
						chb -= 32;
						n = (cha - chb);
					}
					if (n) {
						return n > 0 ? 1 : -1;
					}
				}
				if (cha == 0) {
					break;
				}
				++a;
				++b;
			}
			return 0;
		} else {
			return 1;
		}
	} else {
		return b ? -1 : 0;
	}
}

}}} // end namespace

#endif // sentry
