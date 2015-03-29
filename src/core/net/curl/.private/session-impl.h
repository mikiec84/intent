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

struct session::impl_t /* --<threadsafe */ {

	uint32_t id; // +<final
	channel & channel;
	std::mutex mtx;
	std::map<uint32_t, response::impl_t *> responses;

	// A session can be destroyed directly, or because its containing channel
	// is destroyed. If the latter, then we have a potential deadlock where
	// the channel calls the dtor of all its sessions, and the normal session
	// dtor logic calls the (locked) channel to deregister. We use this member
	// variable to recognize that state and do the right thing.
	bool detached;

	impl_t(class channel & ch);
};


}}}} // end namespace


#endif
