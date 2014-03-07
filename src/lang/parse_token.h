#ifndef _695ce797496541c9b74e67f9c5bfa6a1
#define _695ce797496541c9b74e67f9c5bfa6a1

#include <memory>

enum parse_token_type {
    #define TUPLE(name, number, example, precedence, associativity, comment) \
        ptt_operator##name = number,
    #include "lang/operator_tuples.h"
};

/**
 * Hold information about a single token parsed out
 * from intent code.
 */
class parse_token {
public:
    typedef std::unique_ptr<parse_token> handle;
};

#endif // sentry
