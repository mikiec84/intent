#ifndef _ba49b0b1b64448089044a8e80e8dcd01
#define _ba49b0b1b64448089044a8e80e8dcd01

#include <cstdint>

#include "core/net/curl/callbacks.h"
#include "core/net/curl/headers.h"


namespace intent {
namespace core {
namespace net {
namespace curl {


class channel;
class request;


/**
 * Manage one or more requests against a particular endpoint. A session
 * encapsulates credentials, cookies, and http 1.1 keep-alives--so different
 * sessions against the same server can authenticate differently.
 */
class session {
	struct impl_t;
	impl_t * impl;

	friend class channel;
	friend struct libcurl_callbacks;
	friend class request;
	friend class response;

	void register_response(response *, bool add = true);
	void merge_headers(headers & overrides);

public:

	session();
	session(channel &);
	~session();

	channel & get_channel();
	channel const & get_channel() const;

	uint32_t get_id() const;

	void set_user(char const * user);
	char const * get_user() const;

	void set_password(char const * password);

	response get(char const * url, receive_callback=nullptr);
	response get(char const * url, headers const & headers);

	response put(char const * url);
	response post(char const * url);
	response del(char const * url);
	response options(char const * url);
	response head(char const * url);

	response send(request &&);
};


}}}} // end namespace


#endif // sentry
