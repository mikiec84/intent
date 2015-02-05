#ifndef intent_core_net_curl_private_request_impl_h
#define intent_core_net_curl_private_request_impl_h

#include "core/net/curl/request.h"
#include "core/net/curl/.private/libcurl.h"


namespace intent {
namespace core {
namespace net {
namespace curl {


class session;


struct request::impl_t {

	uint32_t id;
	session * session; // reference; not owned
	request * request; // reference; not owned
	char * verb;	// owned; must delete in dtor
	char * url;		// owned; must delete in dtor
	headers headers;

	impl_t(class session *, class request *, char const * verb, char const * url);
	~impl_t();

};


}}}} // end namespace


#endif
