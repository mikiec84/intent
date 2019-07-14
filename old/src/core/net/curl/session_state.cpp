#include "core/net/curl/session_state.h"
#include "core/util/countof.h"

namespace intent {
namespace core {
namespace net {
namespace curl {


static char const * session_state_names[] = {
    #define tuple(id) #id,
    #include "core/net/curl/session_state.tuples"
};


static constexpr unsigned session_state_count = countof(session_state_names);


char const * get_name_for_session_state(session_state ss) {
    auto i = static_cast<unsigned>(ss);
    return (i < session_state_count) ? session_state_names[i] : nullptr;
}

}}}} // end namespace
