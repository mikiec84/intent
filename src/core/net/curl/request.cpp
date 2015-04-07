#include <atomic>
#include <thread>

#include <asio.hpp>

#include "core/net/curl/.private/libcurl_callbacks.h"
#include "core/net/curl/.private/request-impl.h"
#include "core/net/curl/.private/session-impl.h"
#include "core/net/curl/request.h"
#include "core/util/monotonic_id.h"



using asio::deadline_timer;
using asio::io_service;
using asio::error_code;

using std::atomic;
using std::map;
using std::thread;

using namespace std::placeholders;


namespace intent {
namespace core {
namespace net {
namespace curl {


typedef asio::ip::tcp::socket asio_socket_t;
typedef map<curl_socket_t, asio_socket_t *> socket_map_t;
typedef std::pair<curl_socket_t, asio_socket_t *> socket_pair_t;


static uint32_t get_next_id() {
	static util::monotonic_id<uint32_t> the_generator;
	return the_generator.next();
}


request::impl_t::impl_t(class session * s, class request * r, char const * verb,
	char const * url):
		id(get_next_id()),
		session(s),
		request(r),
		verb(strdup(verb)),
		url(strdup(url)),
		headers(header_is_less) {
}

request::impl_t::~impl_t() {
	free(verb);
	free(url);
}


request::request(session & session, char const * verb, char const * url) :
		impl(new impl_t(&session, this, verb, url)) {
}


request::~request() {
	printf("request %u dtor\n", impl ? get_id() : 9999u);
	delete impl;
}


char const * request::get_verb() const {
	return impl->verb;
}


char const * request::get_url() const {
	return impl->url;
}


channel_handle request::get_channel() {
	return impl->session->get_channel();
}


uint32_t request::get_id() const {
	return impl->id;
}


headers const & request::get_headers() const {
	return impl->headers;
}


headers & request::get_headers() {
	return impl->headers;
}


}}}} // end namespace
