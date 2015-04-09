#ifndef _25b7bb7aad8347e68fe53cdd9c23da02
#define _25b7bb7aad8347e68fe53cdd9c23da02

#include <cstdint>
#include <string>

#include "core/net/headers.h"
#include "core/net/curl/callbacks.h"
#include "core/net/curl/channel.h"
#include "core/net/curl/session.h"
#include "core/net/http_method.h"
#include "core/net/curl/timeout.h"


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

    response(session &);

public:

    ~response();

    session * get_session();

    channel * get_channel();

    void get_url(char const * url);

    uint32_t get_id() const;

    headers const & get_headers() const;
    std::string const & get_body() const;

    uint16_t get_status_code() const;

    void wait(timeout const & t=timeout::standard);
};


}}}} // end namespace


#endif // sentry
