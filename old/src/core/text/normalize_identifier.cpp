#include <sstream>

#include "core/text/normalize_identifier.h"
#include "core/text/strutil.h"

using std::stringstream;


namespace intent {
namespace core {
namespace text {

static inline void normalize_boundary_char(char c, identifier_style out_style,
        unsigned word_idx, stringstream & out, bool last_was_acronym) {

    switch (out_style) {
    case identifier_style::intent_noun:
        if (word_idx) {
            out << ' ';
        }
        out << ascii_to_lower_case(c);
        break;
    case identifier_style::intent_verb:
        if (word_idx) {
            out << ' ' << ascii_to_lower_case(c);
        } else {
            out << ascii_to_upper_case(c);
        }
        break;
    case identifier_style::under:
        if (word_idx) {
            out << '_';
        }
        out << ascii_to_lower_case(c);
        break;
    case identifier_style::camel:
        if (word_idx == 0) {
            out << ascii_to_lower_case(c);
        } else {
            out << ascii_to_upper_case(c);
        }
        break;
    case identifier_style::title:
        out << ascii_to_upper_case(c);
        break;
    case identifier_style::title_under:
        if (word_idx) {
            out << '_';
        }
        out << ascii_to_upper_case(c);
        break;
    case identifier_style::alt_camel: // Like camelCase, but acronyms end with alternate case: useFirstDNSsetting
        out << (last_was_acronym || word_idx == 0 ? ascii_to_lower_case(c) : ascii_to_upper_case(c));
    case identifier_style::alt_title: // Like TitleCase, but acronyms end with alternate case: UseFirstDNSsetting
        out << (last_was_acronym ? ascii_to_lower_case(c) : ascii_to_upper_case(c));
        break;
    }
}

static inline void normalize_internal_char(char c, identifier_style out_style,
        stringstream & out, bool in_acronym) {

    switch (out_style) {
    case identifier_style::intent_verb:
    case identifier_style::intent_noun:
    case identifier_style::under:
        out << ascii_to_lower_case(c);
        break;
    case identifier_style::camel:
    case identifier_style::title:
    case identifier_style::title_under:
    case identifier_style::alt_camel:
    case identifier_style::alt_title:
        out << (in_acronym ? ascii_to_upper_case(c) : ascii_to_lower_case(c));
        break;
    }
}


static inline char const * skip_to_alphanumeric(char const * p, char const * end) {
    for (; p < end; ++p) {
        if (!is_ascii_alphanumeric(*p)) {
            return p;
        }
    }
    return end;
}


std::string normalize_identifier(str_view const & in, identifier_style in_style,
        identifier_style out_style) {

    if (!in) {
        return "";
    }

    char const * end = in.end();
    char const * p = skip_to_alphanumeric(in.begin, end);
    if (p == end) {
        return "";
    }

    stringstream out;

    unsigned word_idx = 0;
    bool last_was_acronym;
    bool in_acronym = false;
    bool word_is_capitalized = false;

    while (p < end) {

        // Here, we're pointing at beginning of a word. Capture some info about
        // our curent state.
        last_was_acronym = in_acronym;
        in_acronym = is_ascii_upper(*p) && p + 1 < end && is_ascii_upper(p[1]);

        // Write the word boundary and its first char.
        normalize_boundary_char(*p, out_style, word_idx, out, last_was_acronym);

        if (++p >= end) {
            break;
        }

        // Now read and write the rest of the current word.
        switch (in_style) {
        case identifier_style::intent_verb:
        case identifier_style::intent_noun:
        case identifier_style::under:
            for (; p < end; ++p) {
                char c = *p;
                if (is_ascii_alphanumeric(c)) {
                    normalize_internal_char(c, out_style, out, in_acronym);
                } else {
                    break;
                }
            }
            break;
        case identifier_style::title:
        case identifier_style::camel:
            // Acronyms inside a camel-case identifier are ended by the
            // penultimate cap char, unless they run until end of string:
            // "useFirstDNSSetting" or "useFirstDNS"
            //             ^                       ^
            if (in_acronym) {
                for (; p + 1 < end && is_ascii_upper(p[1]); ++p);
                if (p + 1 == end) {
                    ++p;
                }
            } else {
                for (; p < end; ++p) {
                    char c = *p;
                    if (is_ascii_upper(c) == word_is_capitalized) {
                        normalize_internal_char(c, out_style, out, in_acronym);
                    } else {
                        break;
                    }
                }
            }
            break;
        case identifier_style::title_under:
        case identifier_style::alt_camel:
        case identifier_style::alt_title:
            {
                char c = *p;
                out << (in_acronym ? ascii_to_upper_case(c) : ascii_to_lower_case(c));
            }
            break;
        }

        ++word_idx;

    }
    return out.str();
}


}}} // end namespace

