#ifndef _e16937443a38431288b08f3e2ae6c3cd
#define _e16937443a38431288b08f3e2ae6c3cd

#include "core/net/curl/.private/request-impl.h"
#include "core/net/curl/.private/response-impl.h"
#include "core/net/curl/.private/session-impl.h"


namespace intent {
namespace core {
namespace net {
namespace curl {


uint32_t get_next_session_id();


inline session::impl_t::impl_t(session * wrapped, class channel * ch):
		id(get_next_session_id()),
		wrapper(wrapped),
		channel(ch),
		mtx(),
		current_request(new request::impl_t(wrapped)),
		current_response(new response::impl_t(wrapped)),
		easy(),
		state(session_state::configuring) {
	error[0] = 0;
}


}}}} // end namespace


#endif
