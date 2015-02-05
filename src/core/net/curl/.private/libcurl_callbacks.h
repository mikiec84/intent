#ifndef intent_core_net_curl_impl_libcurl_callbacks_h
#define intent_core_net_curl_impl_libcurl_callbacks_h

#include <cstdint>

#include "core/net/curl/.private/libcurl.h"
#include "core/net/curl/channel.h"


namespace intent {
namespace core {
namespace net {
namespace curl {


void mcode_or_die(const char *where, CURLMcode code);

struct libcurl_callbacks {
	typedef channel::impl_t chimpl_t;

	static int on_change_timeout(CURLM * multi, long timeout_ms, void * _chimpl);
	static curl_socket_t on_open_socket(void * _chimpl, curlsocktype purpose, curl_sockaddr *address);
	static int on_close_socket(void * _chimpl, curl_socket_t item);
	static int on_socket_update(CURL * easy, curl_socket_t sock, int what, void * _chimpl, void * sockp);
	static int on_progress(void * _session, uint64_t expected_receive_total, uint64_t received_so_far, uint64_t expected_send_total, uint64_t sent_so_far);
	static size_t on_receive_data(void * data, size_t size_per_record, size_t num_records, void * _session);
};


}}}} // end namespace

#endif // sentry
