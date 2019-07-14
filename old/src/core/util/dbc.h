#ifndef _5117d05ca70243e693d2ec052cfec6a9
#define _5117d05ca70243e693d2ec052cfec6a9

#include <stdexcept>

#include "core/util/scope_guard.h"

namespace intent {
namespace core {
namespace util {

enum class contract_type: unsigned {
    pre,
    check,
    post,
};

struct contract_violation: public std::logic_error {
    const contract_type ctype;
    contract_violation(contract_type ctype, char const * expr, char const * file,
        int line, char const * function);
};

/**
 * Define what should happen when internal function contracts are violated.
 */
#ifndef contract_violation_action
#define contract_violation_action(contract_type, expr) \
    throw ::intent::core::util::contract_violation(::intent::core::util::contract_type, #expr, __FILE__, __LINE__, __FUNCTION__)
#endif

#ifndef precondition_violation_action
#define precondition_violation_action(a, b) contract_violation_action(a, b)
#endif

#ifndef intracondition_violation_action
#define intracondition_violation_action(a, b) contract_violation_action(a, b)
#endif

// Caution: The default behavior with postconditions--to throw an exception--
// will terminate the app. This is because we are throwing out of a destructor,
// which is going to call terminate() (see _More Effective C++_ ch 11
// [http://j.mp/1owRmqj]). Callers are free to override this behavior by
// #define-ing postcondition_violation_action in some other way.
#ifndef postcondition_violation_action
#define postcondition_violation_action(a, b) contract_violation_action(a, b)
#endif

#define precondition(expr) \
    if (!(expr)) { \
        precondition_violation_action(contract_type::pre, #expr); \
    }

#define intracondition(expr) \
    if (!(expr)) { \
        intracondition_violation_action(contract_type::check, #expr); \
    }

#define postcondition(expr) \
    on_scope_exit(if (!(expr)) { \
        postcondition_violation_action(contract_type::post, #expr); \
    } )

}}} // end namespace

#endif // sentry
