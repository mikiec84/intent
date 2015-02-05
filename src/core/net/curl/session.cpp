#include <atomic>
#include <map>
#include <mutex>


#include "core/net/curl/.private/session-impl.h"
#include "core/net/curl/channel.h"
#include "core/net/curl/request.h"
#include "core/net/curl/response.h"


using std::atomic;
using std::lock_guard;
using std::map;
using std::mutex;


namespace intent {
namespace core {
namespace net {
namespace curl {


static uint32_t get_next_id() {
	static atomic<uint32_t> the_next_id;
	return the_next_id.fetch_add(1);
}


session::impl_t::impl_t(class channel & ch):
	id(get_next_id()),
	channel(ch),
	responses(),
	mtx() {
}


session::session(channel & ch) :
	impl(new impl_t(ch)) {
	impl->channel.register_session(this);
}


session::session() :
	session(channel::get_default()) {
}


session::~session() {
	impl->channel.register_session(this, false);
	delete impl;
}


channel & session::get_channel() {
	return impl->channel;
}


channel const & session::get_channel() const {
	return impl->channel;
}


uint32_t session::get_id() const {
	return impl->id;
}


void session::register_response(response * r, bool add) {
	if (r) {
		lock_guard<mutex> lock(impl->mtx);
		auto & responses = impl->responses;
		if (add) {
			responses[r->get_id()] = r->impl;
		} else {
			auto x = responses.find(r->get_id());
			if (x != responses.end()) {
				responses.erase(x);
			}
		}
	}
}


void session::set_user(char const * user) {

}


char const * session::get_user() const {
	return nullptr;
}


void session::set_password(char const * password) {

}


void session::merge_headers(headers & overrides) {
}


response session::get(char const * url, receive_callback rcb) {
	class request request(*this, get_http_method_by_index(http_get)->verb, url);
	merge_headers(request.get_headers());
	class response response(std::move(request), rcb);
	return response;
}



}}}} // end namespace
