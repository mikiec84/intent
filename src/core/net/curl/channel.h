#ifndef _35a06a9815d54ed1a42f7525dfd7a8aa
#define _35a06a9815d54ed1a42f7525dfd7a8aa

#include <cstdint>

#include "core/marks/concurrency_marks.h"


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
 * Channels may provide default, overridable configuration (e.g., headers,
 * credentials, callbacks, ) to all sessions that derive from them.
 *
 * Each channel has a single background event/reactor thread (using epoll() on
 * linux, for example) to efficiently dispatch callbacks as data is ready to
 * read or write on a socket; a good rule of thumb is that one channel can
 * efficiently handle, say, up to a few thousand open connections at a time.
 *
 * Channels are thread-safe; instances can be shared safely on multiple threads
 * without additional synchronization.
 */
mark(+, threadsafe)
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

	/**
	 * A channel is initially closed. It opens automatically the first time
	 * someone needs it to do real work. Use this function to test its state.
	 */
	bool is_open() const;

	/**
	 * Simple apps can ignore the channel construct entirely--in which case,
	 * the default channel is always used.
	 */
	static channel & get_default();

	/**
	 * Whenever a new session is created, it is cloned from a prototype provided
	 * by its associated channel. If the prototype session has state such as a
	 * User-Agent header, credentials, callbacks, or debug/verbose configuration,
	 * this lets it propagates--which is both faster, performance-wise, and
	 * cleaner, code-wise.
	 */
	session const * get_prototype_session() const;
	session * get_prototype_session();

	void set_prototype_session(session &&);
	void set_prototype_session(session const &);
};


}}}} // end namespace


#endif // sentry
