#ifndef _ba49b0b1b64448089044a8e80e8dcd01
#define _ba49b0b1b64448089044a8e80e8dcd01

#include <cstdint>

#include "core/net/curl/callbacks.h"
#include "core/net/curl/headers.h"
#include "core/marks/concurrency_marks.h"


namespace intent {
namespace core {
namespace net {
namespace curl {


class channel;
class request;


/**
 * Manage a sequence of zero or more interactions with a single remote endpoint.
 *
 * A session encapsulates accumulated state related to its target endpoint.
 * This includes credentials, cookies, and http 1.1 keep-alives, as well as
 * the callbacks that it triggers in response to events such as progress,
 * completion of send, completion of receive, timeout, and so forth. Each new
 * interaction managed by the session acquires the state accumulated previously.
 *
 * Sessions roughly map onto an open socket and a "curl easy" handle, and as
 * such, they are built to handle only one interaction at a time. Sessions are
 * not threadsafe; you must mutex them if you access them from multiple threads
 * concurrently. However, sessions are easy to clone; this allows you to
 * download multiple resources from the same web server in parallel, for example.
 */
mark(-, threadsafe)
class session {
	struct impl_t;
	impl_t * impl;

	friend class channel;
	friend struct libcurl_callbacks;
	friend class request;
	friend class response;

	void detach();
	void register_response(response *, bool add = true);
	void merge_headers(headers & overrides);

public:

	session();
	session(channel &);
	~session();

	channel & get_channel();
	channel const & get_channel() const;

	uint32_t get_id() const;

	/**
	 * A session can become detached if the channel that it depends on is
	 * closed or destroyed. In such cases, the session and all the requests and
	 * responses that it manages become unusable. This only happens if object
	 * lifetimes are coded incorrectly. In such cases, calls that modify state
	 * of sessions, requests, or responses throw exceptions.
	 */
	bool is_detached() const;

	void set_user(char const * user);
	char const * get_user() const;

	void set_password(char const * password);

	response get(char const * url, receive_callback=nullptr);
	response get(char const * url, headers const & headers);

	response put(char const * url);
	response post(char const * url);
	response del(char const * url);
	response options(char const * url);
	response head(char const * url);

	response send(request &&);

	headers const * get_default_headers() const;
	headers * get_default_headers();
	void set_default_headers(headers &&);

};


}}}} // end namespace


#endif // sentry
