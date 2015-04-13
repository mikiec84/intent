#ifndef _69c081ad860b47edb70efb16e11e7caf
#define _69c081ad860b47edb70efb16e11e7caf

#include <cstdint>
#include <map>
#include <mutex>

#include "core/net/curl/request.h"
#include "core/net/curl/response.h"
#include "core/net/curl/session.h"
#include "core/net/curl/.private/easy.h"


namespace intent {
namespace core {
namespace net {
namespace curl {


struct session::impl_t /* --<threadsafe */ {

	uint32_t id; // +<final
	session * wrapper;
	channel * channel;
	std::mutex mtx;
	request::impl_t * current_request;
	response::impl_t * current_response;
	struct easy easy;
	char error[CURL_ERROR_SIZE];
	session_state state;

	impl_t(session *, class channel *);
	~impl_t();
};


}}}} // end namespace


#include "core/net/curl/.private/session-impl-inline.h"


#endif
