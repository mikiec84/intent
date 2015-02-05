#ifndef intent_core_net_curl_private_session_impl_h
#define intent_core_net_curl_private_session_impl_h

#include <cstdint>
#include <map>
#include <mutex>

#include "core/net/curl/session.h"
#include "core/net/curl/.private/response-impl.h"


namespace intent {
namespace core {
namespace net {
namespace curl {

struct session::impl_t {

	uint32_t id;
	channel & channel;
	std::map<uint32_t, response::impl_t *> responses;
	std::mutex mtx;

	impl_t(class channel & ch);
};


}}}} // end namespace


#endif
