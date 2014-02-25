#include <string.h>

#include "core/pstr.h"

pstr::pstr(char const * txt) : txt(txt), end(strchr(txt, 0)) {
}

size_t pstr::size() const {
    return end - txt;
}