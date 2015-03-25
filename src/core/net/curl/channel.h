#ifndef intent_core_net_curl_channel_h
#define intent_core_net_curl_channel_h


#include <cstdint>


namespace intent {
namespace core {
namespace net {
namespace curl {


class session;


/**
 * Service one or more sessions with servers on the web.
 *
 * A channel is an artifical construct that allows grouping sessions together
 * in whatever way makes sense; there can be a global channel in an app, one
 * channel per unique server, or other postures to suit different purposes.
 * Simple apps only need one channel, and a default is provided and created
 * automatically, allowing channels to mostly be ignored in many use cases.
 *
 * Each channel has a single background event/reactor thread (using epoll() on
 * linux, for example) to efficiently dispatch callbacks as data is ready to
 * read or write on a socket; a good rule of thumb is that one channel can
 * efficiently handle, say, up to about 1000 open connections at a time.
 *
 * Instances of this class can be shared safely on multiple threads without
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
	bool is_open() const;

	static channel & get_default();
};


}}}} // end namespace


#endif // sentry
