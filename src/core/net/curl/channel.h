#ifndef _35a06a9815d54ed1a42f7525dfd7a8aa
#define _35a06a9815d54ed1a42f7525dfd7a8aa

#include <memory>

#include "core/marks/concurrency_marks.h"
#include "core/net/curl/fwd.h"

namespace intent {
namespace core {
namespace net {
namespace curl {


/**
 * Manage events and shared state for one or more sessions with remote servers.
 *
 * A channel is an artifical construct that allows grouping sessions together
 * in whatever way makes sense; there can be a global channel in an app, one
 * channel per unique server, or other postures to suit different purposes.
 * Simple apps only need one channel, and a default is provided and created
 * automatically, allowing channels to be ignored in simple use cases.
 *
 * Each channel owns a separate DNS cache that optimizes host lookups across
 * all the sessions that use it. A channel may also provide default, overridable
 * configuration (e.g., headers, credentials, callbacks, ...) to its sessions.
 *
 * Each channel has a background event/reactor thread to efficiently dispatch
 * callbacks as data is ready to read or write on any of its associated sockets.
 * Since the thread is essentially an event pump, it is rarely blocked, and can
 * scale easily; it is implemented with a best-of-breed eventing mechanism on
 * each platform, such as epoll on linux. A good rule of thumb is that one
 * channel can efficiently handle, say, up to a few thousand open connections
 * at a time.
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
	void attach(session *);
	void detach(unsigned session_id);

public:
	channel();
	~channel();

	/**
	 * Uniquely identify a channel. IDs are auto-assigned, monotonically
	 * incrementing numbers that begin with 0. The default channel may or may
	 * not have id 0, since it is only created on demand.
	 */
	unsigned get_id() const;

	/**
	 * A channel is initially closed. It opens automatically the first time
	 * a consumer needs it to do real work, and remains open from then on. Use
	 * this function to test its state. (Primarily for internal use.)
	 */
	bool is_open() const;

	/**
	 * Simple apps can ignore the channel construct entirely--in which case,
	 * the default channel is always used. This channel is created on demand,
	 * and destroyed (if applicable) on app teardown.
	 */
	static channel_handle get_default();

	/**
	 * Whenever a new session is created, it is cloned from a prototype provided
	 * by its associated channel. If the prototype session has state such as a
	 * User-Agent header, credentials, callbacks, or debug/verbose configuration,
	 * this lets it propagate--which is both faster (performance-wise) and
	 * cleaner (code-wise), than manually duplicating everything.
	 */
	session_handle get_prototype_session();
	void set_prototype_session(session_handle);
};


}}}} // end namespace


#endif // sentry
