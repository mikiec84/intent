#ifndef intent_core_net_curl_impl_libcurl_h
#define intent_core_net_curl_impl_libcurl_h

// All references to the raw libcurl headers should come through this #include
// so we guarantee that curl is used consistently.

#define CURL_STATICLIB 1
#include <curl/curl.h>

#endif // sentry
