#ifndef _69c081ad860b47edb70efb16e11e7caf
#define _69c081ad860b47edb70efb16e11e7caf

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
