//#define CURL_STATICLIB 1
#include <curl/curl.h>

#include "core/net/curl.h"


namespace intent {
namespace core {
namespace net {


// Curl needs to be initialized once, globally, and cleaned up after
// main() exits. Define an object that does this in its ctor and dtor.
struct env {
	env(long flags) {
		curl_global_init(flags);
	}
	~env() {
		curl_global_cleanup();
	}
};


// Create a static instance of the init/cleanup object the first time
// this function is called.
env const & get_env(long flags) {
	static env the_env(flags);
	return the_env;
}


// Any time we call curl_multi_init(), make sure the global init happens
// first, and
CURLM * wrapped_curl_multi_init(long flags = CURL_GLOBAL_ALL) {
	get_env(flags);
	return curl_multi_init();
}


struct curl_channel::data_t {
	CURLM * curlm;

	data_t() : curlm(wrapped_curl_multi_init()) {
	}

	~data_t() {
		curl_multi_cleanup(curlm);
	}
};


curl_channel::curl_channel(): data(new data_t()) {
}


curl_channel::~curl_channel() {
	delete data;
}


}}} // end namespace
