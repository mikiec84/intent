#ifndef _f38c85ad3b2642128140e52ae604e01e
#define _f38c85ad3b2642128140e52ae604e01e

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
