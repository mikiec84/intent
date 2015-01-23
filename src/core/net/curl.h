#ifndef intent_core_net_curl_h
#define intent_core_net_curl_h

#include <cstddef>
#include <cstdint>

namespace intent {
namespace core {
namespace net {

/**
 * Encapsulate one or more concurrent interactions with servers on the web.
 * A channel is an artifical construct that allows grouping sessions together
 * in whatever way makes sense; there can be a global channel in an app, one
 * channel per unique server, or other postures to suit different purposes.
 *
 * Instances of this class can shared safely on multiple threads without
 * additional synchronization.
 */
class curl_channel {
    struct impl_t;
    impl_t * impl;

    friend struct libcurl_callbacks;
    friend class curl_session;

    void open();

public:
    curl_channel();
    ~curl_channel();
};

/**
 * Manage one or more interactions with a particular endpoint, but only one at
 * a time. A session encapsulates credentials, cookies, and http 1.1 keep-alives
 * -- so different sessions against the same server can authenticate differently.
 */
class curl_session {
    struct impl_t;
    impl_t * impl;

    friend struct libcurl_callbacks;
    friend class curl_channel;

public:

    typedef size_t (*receive_callback)(curl_session &, void * bytes, size_t byte_count);
    typedef int (*progress_callback)(curl_session &, uint64_t expected_receive_total,
        uint64_t received_so_far, uint64_t expected_send_total, uint64_t sent_so_far);

    curl_session(curl_channel &, receive_callback, progress_callback = nullptr);
    ~curl_session();

    void set_url(char const * url);
    void get();
};

}}} // end namespace

#endif // sentry
