#ifndef _3177ced09a404921a53553991123de3e
#define _3177ced09a404921a53553991123de3e

#include "core/net/curl/.private/libcurl.h"


namespace intent {
namespace core {
namespace net {
namespace curl {


/**
 * Wrap a CURLSH * (curl shared object) so we can guarantee correct
 * initialization. This is basically a specialized, very simple smart pointer.
 */
struct share {
    CURLSH * _wrapped;
    share() : _wrapped(curl_share_init()) {}
    ~share() {
        curl_share_cleanup(_wrapped);
    }
    // Allow this object to be used as if it were a CURL *.
    operator CURLSH *() { return _wrapped; }
    CURLSH * operator ->() { return _wrapped; }
};


}}}} // end namespace


#endif // sentry
