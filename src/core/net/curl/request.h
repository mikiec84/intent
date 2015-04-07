#ifndef _4bee73c5a8e642a38e1fbb4791446ab5
#define _4bee73c5a8e642a38e1fbb4791446ab5

#include <cstdint>

#include "core/net/curl/callbacks.h"
#include "core/net/curl/channel.h"
#include "core/net/curl/headers.h"
#include "core/net/curl/session.h"
#include "core/net/http_method.h"

namespace intent {
namespace core {
namespace net {
namespace curl {


/**
 * Encapsulate a single request to a remote endpoint.
 *
 * Requests are not thread-safe; you must mutex them for concurrent access to
 * a single instance from more than one thread.
 */
// -<threadsafe
class request {
	struct impl_t;
	impl_t * impl;

	friend class response;
	friend class session;

	request(session &, char const * verb, char const * url);

public:

	~request();

	session_handle get_session();
	const_session_handle get_session() const;
	channel_handle get_channel();
	char const * get_verb() const;
	char const * get_url() const;
	uint32_t get_id() const;

	headers const & get_headers() const;
	headers & get_headers();
};


}}}} // end namespace


#endif // sentry
