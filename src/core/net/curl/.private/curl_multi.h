#ifndef CURL_MULTI_H
#define CURL_MULTI_H

#include "core/net/curl/.private/libcurl.h"


namespace intent {
namespace core {
namespace net {
namespace curl {


struct env;
env const & get_env(long flags);


/**
 * Wrap a CURLM * so we can guarantee correct initialization. This is basically
 * a specialized, very simple smart pointer.
 */
struct curl_multi {
	CURLM * _wrapped;
	curl_multi(long flags = CURL_GLOBAL_ALL) {
		// Any time we call curl_multi_init(), make sure the global init happens
		// first. This gives us JIT initialization of curl, with destruction after
		// main()...
		get_env(flags);
		_wrapped = curl_multi_init();
	}
	~curl_multi() {
		curl_multi_cleanup(_wrapped);
	}
	// Allow this object to be used as if it were a CURLM *.
	operator CURLM *() { return _wrapped; }
	CURLM * operator ->() { return _wrapped; }
};


}}}} // end namespace


#endif // CURL_MULTI_H
