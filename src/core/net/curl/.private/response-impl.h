#ifndef _e2530873fce14f25be6d86d14358524c
#define _e2530873fce14f25be6d86d14358524c

#include "core/net/curl/request.h"
#include "core/net/curl/response.h"
#include "core/net/curl/.private/libcurl.h"


namespace intent {
namespace core {
namespace net {
namespace curl {


class session;


struct response::impl_t mark(-, threadsafe) {

	uint32_t id; // +<final;
	response * response;
	request_handle request;
	session_handle session;
	CURL * curl; // +<final, --<owned_elsewhere
	char error[CURL_ERROR_SIZE];
	receive_callback receive_func; // +<final
	progress_callback progress_func; // +<final
	headers headers;
	char * received_bytes; // --<owned_elsewhere
	size_t allocated_byte_count;
	size_t received_byte_count;
	size_t expected_receive_total;
	size_t sent_byte_count;
	size_t expected_send_total;
	uint16_t status_code;

	impl_t(class response *, class request &&, receive_callback, progress_callback);
	~impl_t();

};


}}}} // end namespace


#endif
