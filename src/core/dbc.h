#ifndef DBC_H
#define DBC_H

#include <stdexcept>

#include "scope_guard.h"

enum class contract_type: unsigned {
    pre,
    check,
    post,
};

struct contract_violation: public std::logic_error {
    const contract_type ctype;
    contract_violation(contract_type ctype, char const * expr, char const * file,
        int line);
};

/**
 * Define what should happen when internal function contracts are violated.
 */
#ifndef CONTRACT_VIOLATION_ACTION
#define CONTRACT_VIOLATION_ACTION(contract_type, expr) \
    throw contract_violation(contract_type, #expr, __FILE__, __LINE__)
#endif

#ifndef PRECONDITION_VIOLATION_ACTION
#define PRECONDITION_VIOLATION_ACTION(a, b) CONTRACT_VIOLATION_ACTION(a, b)
#endif

#ifndef CHECK_VIOLATION_ACTION
#define CHECK_VIOLATION_ACTION(a, b) CONTRACT_VIOLATION_ACTION(a, b)
#endif

// Caution: The default behavior with POSTCONDITIONs--to throw an exception--
// will terminate the app. This is because we are throwing out of a destructor,
// which is going to call terminate() (see _More Effective C++_ ch 11
// [http://j.mp/1owRmqj]). Callers are free to override this behavior by
// #define-ing POSTCONDITION_VIOLATION_ACTION in some other way.
#ifndef POSTCONDITION_VIOLATION_ACTION
#define POSTCONDITION_VIOLATION_ACTION(a, b) CONTRACT_VIOLATION_ACTION(a, b)
#endif

#define PRECONDITION(expr) \
    if (!(expr)) { \
        PRECONDITION_VIOLATION_ACTION(contract_type::pre, #expr); \
    }

#define CHECK(expr) \
    if (!(expr)) { \
        CHECK_VIOLATION_ACTION(contract_type::check, #expr); \
    }

#define POSTCONDITION(expr) \
    on_scope_exit(if (!(expr)) { \
        POSTCONDITION_VIOLATION_ACTION(contract_type::post, #expr); \
    } )

#endif // sentry
