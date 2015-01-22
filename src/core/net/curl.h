#ifndef intent_core_net_curl_h
#define intent_core_net_curl_h

namespace intent {
namespace core {
namespace net {

/**
 * Encapsulate one or more concurrent interactions with servers on the web.
 */
class curl_channel {
	struct data_t;
	data_t * data;

public:
	curl_channel();
	~curl_channel();

};

/**
 * Manage one or more interactions with a particular endpoint, one at a time.
 */
class curl_session {
	struct data_t;
	data_t * data;

public:
	curl_session(curl_channel &);
	~curl_session();
};

}}} // end namespace

#endif // sentry
