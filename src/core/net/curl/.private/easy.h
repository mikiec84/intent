#ifndef _f37e5b11b9104df39cba822ba7b1820e
#define _f37e5b11b9104df39cba822ba7b1820e

#include "core/net/curl/.private/libcurl.h"


namespace intent {
namespace core {
namespace net {
namespace curl {


/**
 * Wrap a CURL * (curl easy session) so we can guarantee correct initialization.
 * This is basically a specialized, very simple smart pointer.
 */
struct easy {
    CURL * _wrapped;
    easy() : _wrapped(curl_easy_init()) {}
    ~easy() {
        curl_easy_cleanup(_wrapped);
    }
    // Allow this object to be used as if it were a CURL *.
    operator CURL *() { return _wrapped; }
    CURL * operator ->() { return _wrapped; }
};


}}}} // end namespace


#endif // sentry
