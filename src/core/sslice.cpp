#include <iostream>

#include "core/sslice.h"

namespace intent {
namespace core {

const sslice null_sslice;

}} // end namespace

using std::ostream;

ostream & operator <<(ostream & out, const intent::core::sslice & slice) {
    if (slice) {
        for (char const * p = slice.begin; p != slice.end; ++p) {
            out << *p;
        }
    }
    return out;
}

