#include "core/sslice.h"
#include "core/escape_sequence.h"

using std::string;

namespace intent {
namespace core {

std::string expand_escape_sequences(sslice const & s) {
    char buf[8];
    std::string result;
    result.reserve(s.size());
    for (char const * p = s.begin; p < s.end; ++p) {
        if (*p == '\\') {
            codepoint_t cp;
            p = scan_unicode_escape_sequence(p + 1, cp) - 1;
            if (p < s.end) {
                char * bufp = buf;
                size_t bufsize = sizeof(buf);
                cat_utf8_or_escape_sequence(bufp, bufsize, cp);
                result += buf;
            } else {
                break;
            }
        } else {
            result += *p;
        }
    }
    return result;
}

bool should_escape_in_utf8_string_literals(uint32_t cp) {
    if (cp < 32) {
        return !(cp == 9 || cp == 10 || cp == 13);
    } else if (cp <= 0x7f) {
        return (cp == '\\' || cp == '"' || cp == 0x7f);
    }
    return false;
}

std::string insert_escape_sequences(sslice const & s, should_escape_func should_escape) {
    std::string result;
    result.reserve(s.size());
    codepoint_t cp;
    char buf[12];
    for (const char * p = s.begin; p != s.end;) {
        p = get_codepoint_from_utf8(p, cp);
        if (should_escape(cp)) {
            char * bufp = buf;
            size_t bufsize = sizeof(buf);
            if (add_escape_sequence(bufp, bufsize, cp)) {
                result += buf;
            }
        } else {
            if (cp < 0x7f) {
                result += static_cast<char>(cp);
            } else {
                char * bufp = buf;
                size_t bufsize = sizeof(buf);
                if (add_codepoint_to_utf8(bufp, bufsize, cp)) {
                    result += buf;
                }
            }
        }
    }
    return result;
}

}} // end namespace
