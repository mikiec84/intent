#ifndef intent_lang_parser_h
#define intent_lang_parser_h

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
