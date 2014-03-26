#include <cstring>

#include "lang/code_site.h"

namespace {

inline char * copy_of(char const * src) {
    size_t len = strlen(src);
    auto copy = new char[len + 1];
    strncpy(copy, src, len + 1);
    return copy;
}

} // end anonymous namespace

namespace intent {
namespace lang {

code_site::code_site(char const * compilation_unit_url, unsigned line, unsigned offset,
                     url_copy_strategy url_strategy) :
    compilation_unit_url(compilation_unit_url && (url_strategy == make_private_copy) ?
                             copy_of(compilation_unit_url) :
                             compilation_unit_url),
    line_number(line), offset_on_line(offset),
    url_strategy(compilation_unit_url ? url_strategy : dont_copy) {
}

code_site::~code_site() {
    if (url_strategy == make_private_copy) {
        delete[] const_cast<char *>(compilation_unit_url);
    }
}

} // end namespace lang
} // end namespace intent

