#ifndef intent_core_net_curl_private_response_impl_h
#define intent_core_net_curl_private_response_impl_h

#include "core/net/curl/request.h"
#include "core/net/curl/response.h"
#include "core/net/curl/.private/libcurl.h"


namespace intent {
namespace core {
namespace net {
namespace curl {


class session;


struct response::impl_t {

	uint32_t id;
	response * response;	// backref; not owned
	request request;		// backref; not owned
	CURL * curl;
	char error[CURL_ERROR_SIZE];
	receive_callback receive_func;
	progress_callback progress_func;
	headers headers;
	char * received_bytes;
	size_t allocated_byte_count;
	size_t received_byte_count;
	size_t expected_receive_total;
	size_t sent_byte_count;
	size_t expected_send_total;
	int status_code;

	impl_t(class response *, class request &&, receive_callback, progress_callback);
	~impl_t();

};


}}}} // end namespace


#endif
