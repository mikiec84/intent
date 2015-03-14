#ifndef _6752c1ebc9f111e4a5cc3c15c2ddfeb4
#define _6752c1ebc9f111e4a5cc3c15c2ddfeb4

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
