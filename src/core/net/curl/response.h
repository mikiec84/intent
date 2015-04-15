#ifndef _25b7bb7aad8347e68fe53cdd9c23da02
#define _25b7bb7aad8347e68fe53cdd9c23da02

#include <cstdint>
#include <string>

#include "core/net/headers.h"
#include "core/net/curl/callbacks.h"
#include "core/net/curl/channel.h"
#include "core/net/curl/session.h"
#include "core/net/http_method.h"


namespace intent {
namespace core {
namespace net {
namespace curl {

/**
 * Encapsulate a single response from a remote endpoint.
 *
 * Responses are not thread-safe; you must mutex them for concurrent access to
 * a single instance from more than one thread.
 */
// -<threadsafe
class response {
    struct impl_t;
    impl_t * impl;

    friend struct libcurl_callbacks;
    friend class request;
    friend class session;

    response(session *);
    response(impl_t *);

public:

    response(response const &);
    response(response &&);
    ~response();

    session & get_session();
    session const & get_session() const;

    channel & get_channel();
    channel const & get_channel() const;

    void get_url(char const * url);

    uint32_t get_id() const;

    headers const & get_headers() const;
    std::string const & get_body() const;

    uint16_t get_status_code() const;

    /**
     * Wait until response is ready or timeout occurs. When this method returns,
     * we are guaranteed that a session's state will not change again unless/
     * until we manually change it; the final attributes of its current request
     * and response, such as a status code, effective URL, headers, and response
     * body--are available for inspection. Note that this may include an error
     * state, not just success.
     *
     * Normally, wait() will be called when the associated session is configured
     * and a request is underway. However, it is also possible to call wait()
     * late, when a response has already finished--in which case, the method
     * returns true immediately.
     *
     * @param timeout_millisecs Defaults to one minute. Infinite timeouts are
     *     strongly discouraged, because they are dangerous; however, they may
     *     sometimes be necessary to support days-long downloads, and can be
     *     simulated by using a big number here. Any number less than 1000 is
     *     probably unwise, and any number less than 100 is rounded up to 100.
     *
     * @return true if response is ready, false if timeout occurs or wait was
     *     called prematurely (no work was underway).
     */
    bool wait(unsigned timeout_millisecs=60000);
};


}}}} // end namespace


#endif // sentry
