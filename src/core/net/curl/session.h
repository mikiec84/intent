#ifndef _ba49b0b1b64448089044a8e80e8dcd01
#define _ba49b0b1b64448089044a8e80e8dcd01

#include <cstdint>
#include <memory>

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
 * Handle a stateful sequence of one or more interactions with a single remote
 * endpoint.
 *
 * Each new interaction encapsulated by the session builds upon the state
 * accumulated previously. This includes credentials, cookies, keep-alives, and
 * TLS handshakes.
 *
 * Sessions also remember which callbacks to invoke in response to events such
 * as progress, completion of send, completion of receive, timeout, and so
 * forth.
 *
 * Sessions roughly map onto an open socket and a "curl easy", and as
 * such, they are built to handle only one interaction at a time. However, it
 * is easy to clone a session, avoiding repetitive configuration. Also, the
 * @ref shared_cookies and @ref shared_tls_handshake classes allow some aspects
 * of a session's state to be reused with little overhead. Using these
 * mechanisms, it is possible to efficiently download multiple resources from
 * the same web server in parallel, for example.
 *
 * The underlying state encapuslated by session objects cannot come and go on a
 * whim, especially when portions of that state are shared across multiple
 * interactions. For this reason, session objects are owned by their associated
 * channel; all access to them passes through lightweight ref-counted handles.
 * If a session handle goes out of scope, and it is proxying a pending transfer,
 * the transfer continues to run, and an equivalent session handle can still be
 * found in the channel's list of sessions until it finalizes. Any callbacks
 * associated with that session will continue to fire in the meantime. This
 * means you do not have to convolute code to keep a handle in scope, just to
 * make sure work finishes. You can "set it and forget it" if that's desirable.
 * However, ref counting also means that holding a handle can keep resources
 * allocated beyond their useful lifetime. Release handles when you don't need
 * them anymore. (See the @ref get_id() method for an easy way to track sessions
 * without holding handles, in a "weak reference" style.)
 *
 * Sessions are thread-safe; instances can be shared safely on multiple threads
 * without additional synchronization.
 */
mark(+, threadsafe)
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

    /**
     * Identifies the underlying state encapsulated by this session. IDs are
     * monotonically incrementing numbers
     * @return
     */
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

typedef std::shared_ptr<session> session_handle;


}}}} // end namespace


#endif // sentry
