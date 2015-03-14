#ifndef _6752a9f0c9f111e4a3223c15c2ddfeb4
#define _6752a9f0c9f111e4a3223c15c2ddfeb4

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
