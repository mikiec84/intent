#ifndef _67548c1ec9f111e4900e3c15c2ddfeb4
#define _67548c1ec9f111e4900e3c15c2ddfeb4

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
