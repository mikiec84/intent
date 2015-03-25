#ifndef _d1c7ed60cc3d4a079e7232f44d26a225
#define _d1c7ed60cc3d4a079e7232f44d26a225

#include <cstddef>

#include "core/text/str_view.h"

namespace intent {
namespace lang {

/**
 * Parse a chunk of intent code.
 */
class parser {
    struct impl_t;
    impl_t * impl;

public:
    parser(char const * begin, char const * end);
    parser(char const * begin, size_t length);
    parser(core::text::str_view const & txt);
    ~parser();
};

}} // end namespace

#endif // sentry
