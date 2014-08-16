#include <string.h>

#include "core/pstr.h"

namespace intent {
namespace core {

pstr::pstr(char const * txt) : txt(txt), end(strchr(txt, 0)) {
}

size_t pstr::size() const {
    return end - txt;
}

}} // end namespace
