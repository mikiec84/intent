#ifndef intent_core_net_curl_h
#define intent_core_net_curl_h

#include <cstddef>
#include <cstdint>
#include <map>
#include <string>

namespace intent {
namespace core {
namespace net {
namespace curl {

class channel;
class session;
class request;

typedef std::map<std::string, std::string> params;

/**
 * Describe the state of an interaction that is sending or receiving bytes.
 */
enum class stream_state {
    /** Not done yet. */
    more,
    /** All bytes have been streamed. */
    finished,
    /** No more bytes will be accepted/provided, but this represents an error,
     * not success. */
    premature_close,
};


/**
 * A function that is called when a request receives new data. This function
 * typically writes the bytes to a file.
 *
 * @return The number of bytes handled. Any value different from byte_count
 *     signals an error and triggers the abandonment of the transfer.
 */
typedef size_t (*receive_callback)(request &, void * bytes, size_t byte_count);


/**
 * A function that is called when a request has an update about how many bytes
 * have been sent and/or received.
 */
typedef int (*progress_callback)(request &, uint64_t expected_receive_total,
    uint64_t received_so_far, uint64_t expected_send_total, uint64_t sent_so_far);


/**
 * A function that is called when a request is ready to accept more data for
 * sending. The callback provides as much data as it has by setting the
 * OUT parameters.
 *
 * @param bytes (OUT) To provide data, the callback points this OUT parameter to
 *     bytes that are ready to transfer. This value is ignored if the callback
 *     returns stream_state::premature_close or if byte_count is set to 0.
 * @param byte_count (OUT) To provide data, the callback sets this OUT parameter
 *     to the number of bytes that are ready to transfer. This value is ignored
 *     if the callback returns stream_state::premature_close. It is possible to
 *     return 0 for byte_count and to return stream_state::more; in such cases,
 *     the curl infrastructure goes into a sleep+poll loop waiting for more
 *     data to become available.
 * @return stream_state::finished if, after processing byte_count bytes, the
 *     transfer should be considered complete; stream_state::more if the
 *     callback should be invoked again to get more data after processing what
 *     is being returned this time; and stream_state::premature_close if
 *     something went wrong and the transfer should be abandoned.
 */
typedef stream_state (*send_callback)(request &, void *& bytes, size_t & byte_count);

/**
 * Encapsulate one or more concurrent interactions with servers on the web.
 * A channel is an artifical construct that allows grouping sessions together
 * in whatever way makes sense; there can be a global channel in an app, one
 * channel per unique server, or other postures to suit different purposes.
 * Channels share accepted security certificates, cache, and other state that
 * might accumulate across multiple sessions with multiple remote hosts.
 *
 * Instances of this class can shared safely on multiple threads without
 * additional synchronization.
 */
class channel {
    struct impl_t;
    impl_t * impl;

    friend class session;

    void open();
    void register_session(session *, bool add=true);

public:
    channel();
    ~channel();

    uint32_t get_id() const;

    static channel & get_default();
};

/**
 * Manage one or more requests against a particular endpoint. A session
 * encapsulates credentials, cookies, and http 1.1 keep-alives--so different
 * sessions against the same server can authenticate differently.
 */
class session {
    struct impl_t;
    impl_t * impl;

    friend class request;

    void register_request(request *, bool add = true);

public:

    session();
    session(channel &);
    ~session();

    channel & get_channel();
    channel const & get_channel() const;

    uint32_t get_id() const;
};


/**
 * Encapsulate a single interaction with a remote endpoint.
 */
class request {
    struct impl_t;
    impl_t * impl;

public:

    request(channel &, receive_callback, progress_callback = nullptr);
    ~request();

    session & get_session();
    session const & get_session() const;

    void get_url(char const * url);
    void get();
};


}}}} // end namespace

#endif // sentry
