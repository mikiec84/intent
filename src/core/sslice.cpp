#include <iostream>

#include "core/sslice.h"

const sslice null_sslice;

using std::ostream;

ostream & operator <<(ostream & out, const sslice & slice) {
    if (slice) {
        for (char const * p = slice.begin; p != slice.end; ++p) {
            out << *p;
        }
    }
    return out;
}

