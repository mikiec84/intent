#ifndef _c62d87afcff042618aad63d5f77e6c0f
#define _c62d87afcff042618aad63d5f77e6c0f

#include <cstdint>

namespace intent {
namespace core {
namespace net {
namespace curl {


/**
 * The states that a session passes through during its lifecycle.
 */
enum class session_state: uint8_t {
    #define tuple(id) id,
    #include "core/net/curl/session_state.tuples"
};


char const * get_name_for_session_state(session_state);

}}}} // end namespace


#endif // sentry
