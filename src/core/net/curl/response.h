#ifndef _25b7bb7aad8347e68fe53cdd9c23da02
#define _25b7bb7aad8347e68fe53cdd9c23da02

#include <cstdint>

#include "core/net/curl/callbacks.h"
#include "core/net/curl/channel.h"
#include "core/net/curl/headers.h"
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
    friend class session;

    void detach();

    response(request &&, receive_callback = nullptr, progress_callback = nullptr);

    static uint64_t store_bytes_in_response(response &, void * bytes, uint64_t byte_count);

    static int update_progress_in_response(response &, uint64_t expected_receive_total,
            uint64_t received_so_far, uint64_t expected_send_total, uint64_t sent_so_far);

public:

    // Responses are usually created by calling relevant methods such as "get" or
    // "post", on a session. The move ctor only exists to allow return-by-value.

    response(response &&);
    ~response();

    session & get_session();
    session const & get_session() const;

    channel & get_channel();
    channel const & get_channel() const;

    void get_url(char const * url);

    uint32_t get_id() const;

    headers const & get_headers() const;
    headers & get_headers();

    uint16_t get_status_code() const;
    void wait(timeout const & t=timeout::standard);
};


}}}} // end namespace


#endif // sentry
