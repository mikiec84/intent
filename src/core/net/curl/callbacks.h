#ifndef intent_core_net_curl_callbacks_h
#define intent_core_net_curl_callbacks_h

#include <cstddef>
#include <cstdint>


namespace intent {
namespace core {
namespace net {
namespace curl {


class response;
class session;

/**
 * Describe the state of an interaction that is sending or receiving bytes.
 */
enum class stream_state {
	/** Not done yet. */
	more,
	/** All bytes have been streamed. */
	finished,
	/** No more bytes can be streamed, but this represents an error,
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
typedef size_t (*receive_callback)(response &, void * bytes, size_t byte_count);


/**
 * A function that is called when a request has an update about how many bytes
 * have been sent and/or received.
 */
typedef int (*progress_callback)(response &, uint64_t expected_receive_total,
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
typedef stream_state (*send_callback)(response &, void *& bytes, size_t & byte_count);


}}}} // end namespace


#endif // sentry
