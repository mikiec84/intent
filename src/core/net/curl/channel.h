#ifndef _6752ce8ac9f111e4a5af3c15c2ddfeb4
#define _6752ce8ac9f111e4a5af3c15c2ddfeb4

#include <cstdint>


namespace intent {
namespace core {
namespace net {
namespace curl {


class session;


/**
 * Encapsulate one or more concurrent interactions with servers on the web.
 * A channel is an artifical construct that allows grouping sessions together
 * in whatever way makes sense; there can be a global channel in an app, one
 * channel per unique server, or other postures to suit different purposes.
 * Channels share accepted security certificates, cache, and other state that
 * might accumulate across multiple sessions with multiple remote hosts.
 *
 * Instances of this class can shared safely on multiple threads without
 * additional synchronization.
 */
class channel {
	struct impl_t;
	impl_t * impl;

	friend struct libcurl_callbacks;
	friend class request;
	friend class response;
	friend class session;

	void open();
	void register_session(session *, bool add=true);

public:
	channel();
	~channel();

	uint32_t get_id() const;

	static channel & get_default();
};


}}}} // end namespace


#endif // sentry
