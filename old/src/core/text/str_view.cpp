#include <iostream>

#include "core/text/str_view.h"

using std::ostream;

ostream & operator <<(ostream & out, const intent::core::text::str_view & slice) {
    if (slice) {
        auto end = slice.end();
        for (char const * p = slice.begin; p != end; ++p) {
            out << *p;
        }
    }
    return out;
}

ostream & operator <<(ostream & out, const intent::core::text::wstr_view & slice) {
    if (slice) {
        auto end = slice.end();
        for (wchar_t const * p = slice.begin; p != end; ++p) {
            out << *p;
        }
    }
    return out;
}
