#ifndef intent_core_net_curl_impl_libcurl_h
#define intent_core_net_curl_impl_libcurl_h

// All references to the raw libcurl headers should come through this #include
// so we guarantee that curl is used consistently.

#define CURL_STATICLIB 1
#include <curl/curl.h>

#define CALL_CURL(f, args) \
	printf("Calling %s %s in %s, %s line %d\n", #f, #args, __func__, __FILE__, __LINE__); \
	fflush(stdout); \
	f args

#define CALL_CURL_AND_ASSIGN(f, args, ret) \
	printf("Calling %s %s in %s, %s line %d\n", #f, #args, __func__, __FILE__, __LINE__); \
	fflush(stdout); \
	ret = f args

#endif // sentry
