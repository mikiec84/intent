#ifndef _6752e3cac9f111e49f8c3c15c2ddfeb4
#define _6752e3cac9f111e49f8c3c15c2ddfeb4

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
 */
class request {
	struct impl_t;
	impl_t * impl;

	friend class response;
	friend class session;

	request(session &, char const * verb, char const * url);

public:

	// Requests are usually created by calling relevant methods such as "get" or
	// "post", on a session. The move ctor only exists to allow return-by-value.

	request(request &&);
	~request();

	session & get_session();
	session const & get_session() const;
	channel & get_channel();
	channel const & get_channel() const;
	char const * get_verb() const;
	char const * get_url() const;
	uint32_t get_id() const;
	headers const & get_headers() const;
	headers & get_headers();
};


}}}} // end namespace


#endif // sentry
