#ifndef _8942484fff834e998a661fe73dcab8d9
#define _8942484fff834e998a661fe73dcab8d9

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
		CALL_CURL_AND_ASSIGN(curl_multi_init,(), _wrapped);
	}
	~curl_multi() {
		CALL_CURL(curl_multi_cleanup,(_wrapped));
	}
	// Allow this object to be used as if it were a CURLM *.
	operator CURLM *() { return _wrapped; }
	CURLM * operator ->() { return _wrapped; }
};


}}}} // end namespace


#endif // CURL_MULTI_H
