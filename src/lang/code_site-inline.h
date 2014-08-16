#ifndef _code_site_inline_h_968d0016b82e4fff8f6bb21e622c8197
#define _code_site_inline_h_968d0016b82e4fff8f6bb21e622c8197

#include <cstring>

#ifndef code_site_h_e2e12222496d404491498772700655f4
#include "lang/code_site.h"
#endif

namespace intent {
namespace lang {

/**
 * By default, code_site does not make a private copy of the
 * url it's given. This allows a single compilation unit to generate
 * lots of errors or warnings, without allocating memory or mutexing the
 * global heap to copy a long path to a file over and over again. However,
 * it also means that a code_site object must always have a lifetime that
 * fits within the lifetime of whatever larger context owns its url. If
 * it's desirable to create a code_site with an independent lifetime,
 * use make_private_copy_strategy instead.
 */
struct no_copy_strategy {
    static char const * copy(char const * p) { return p; }
    static void deallocate(char const *) {}
};

struct make_private_copy_strategy {
    static char const * copy(char const * p) {
        if (p == nullptr) return p;
        char * alloced = new char[strlen(p) + 1];
        strcpy(alloced, p);
        return alloced;
    }
    static void deallocate(char const * p) {
        if (p) {
            delete[] const_cast<char *>(p);
        }
    }
};

template <typename T>
inline code_site<T>::code_site(char const * compilation_unit_url, unsigned line, unsigned offset) :
    compilation_unit_url(T::copy(compilation_unit_url)),
    line_number(line), offset_on_line(offset) {
}

template <typename T>
inline code_site<T>::~code_site() {
    T::deallocate(compilation_unit_url);
}

} // end namespace lang
} // end namespace intent

#endif // sentry
