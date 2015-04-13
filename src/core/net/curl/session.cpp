#include <atomic>
#include <map>
#include <mutex>


#include "core/net/curl/.private/libcurl_callbacks.h"
#include "core/net/curl/.private/channel-impl.h"
#include "core/net/curl/.private/request-impl.h"
#include "core/net/curl/.private/session-impl.h"
#include "core/net/curl/channel.h"
#include "core/net/curl/request.h"
#include "core/net/curl/response.h"
#include "core/net/curl/state_error.h"
#include "core/net/http_method.h"
#include "core/util/monotonic_id.h"
#include "core/text/interp.h"

using std::atomic;
using std::lock_guard;
using std::map;
using std::mutex;

using intent::core::text::interp;

using namespace intent::core::net;


namespace intent {
namespace core {
namespace net {
namespace curl {


uint32_t get_next_session_id() {
	static intent::core::util::monotonic_id<uint32_t> the_generator;
	return the_generator.next();
}


session::impl_t::~impl_t() {

	// We don't need to lock in this method, because if we get here, then
	// nobody can possibly have a pointer or reference to the object. We know
	// this, because all access to session objects goes through ref-counted
	// smart pointers.
	//lock_guard<mutex> lock(impl->mtx);

	// Normally, a session's lifespan fits neatly inside the lifespan of its
	// channel. In such cases, we will have a valid handle to a channel at this
	// point, and we should tell the channel to remove us from its list.
	// The unusual case is that a channel is dying before we do. In that case,
	// its destructor will have broken our backref, and will be removing us
	// from its list already, so all we do is our own cleanup.
	if (channel) {
		channel->detach(id);
	}

	// Release smart pointer to request.
	if (current_request) {
		// If we have a current request, break its link to us, so that when we
		// fire its destructor soon, it won't try to contact us to break our link.
		current_request->session = nullptr;
		// This may cause the request's dtor to fire, if we're the only one
		// holding a handle to the request. But it's also possible that the
		// request will live on because a handle to it is also held elsewhere.
		// In fact, this is likely, since the response probably holds a handle.
		current_request->release_ref();
		current_request = nullptr;
	}

	// Now do a similar release for response.
	if (current_response) {
		current_response->session = nullptr;
		current_response->release_ref();
		current_request = nullptr;
	}
}


session::session(class channel & ch) :
		impl(new impl_t(this, &ch)) {
	// We have to do this in the body of the ctor, because only this outer
	// class, not the impl, has the right "this" pointer.
	ch.attach(this);
	fprintf(stderr, "session %u ctor\n", get_id());
}


session::session() :
	session(channel::get_default()) {
}


static inline void copy_error(char * buf, char const * txt, size_t bytes_to_copy) {
	static constexpr size_t max_error_bytes_to_copy = CURL_ERROR_SIZE - 1;

	if (bytes_to_copy && txt && *txt) {
		bytes_to_copy = std::min(max_error_bytes_to_copy, bytes_to_copy);
		strncpy(buf, txt, bytes_to_copy);
		buf[bytes_to_copy] = 0;
	} else {
		buf[0] = 0;
	}
}


void session::set_error(char const * txt) {
	copy_error(impl->error, txt, txt ? strlen(txt): 0);
}


void session::set_error(std::string const & txt) {
	copy_error(impl->error, txt.c_str(), txt.size());
}


static void set_curl_verb(CURL * easy, char const * verb) {
	static constexpr uint8_t EMPTY_DATA[] = {0};

	auto method = get_http_method_by_name(verb);
	switch (method->id) {
	case http_get:
		curl_easy_setopt(easy, CURLOPT_HTTPGET, 1L);
		return;
	case http_post:
		curl_easy_setopt(easy, CURLOPT_HTTPPOST, &EMPTY_DATA);
		curl_easy_setopt(easy, CURLOPT_POSTFIELDSIZE, 0L);
		return;
	case http_put:
	case http_head:
	case http_delete:
	case http_options:
	default:
		break;
	}
}

#if 0
static uint64_t store_bytes_in_response(response &, void * bytes, uint64_t byte_count);

static int update_progress_in_response(response &, uint64_t expected_receive_total,
		uint64_t received_so_far, uint64_t expected_send_total, uint64_t sent_so_far);
#endif

response session::send() {
	lock_guard<mutex> lock(impl->mtx);

	auto ch = impl->channel ? impl->channel->impl : nullptr;
	if (!ch) {
		throw state_error("Session is detached from channel.");
	}

	auto state = impl->state;
	if (state != session_state::configuring) {
		throw state_error(interp("Session state is '{1}'; request cannot be sent.", {get_name_for_session_state(state)}));
	}

	auto req = impl->current_request;
	auto url = req ? req->url : nullptr;
	if (!url || !*url) {
		throw state_error("No url has been configured in the request.");
	}

	CURL * easy = impl->easy;
	curl_easy_setopt(easy, CURLOPT_URL, url);
	set_curl_verb(easy, req->verb);
	curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION, libcurl_callbacks::on_receive_data);
	curl_easy_setopt(easy, CURLOPT_WRITEDATA, impl->current_response);
	curl_easy_setopt(easy, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(easy, CURLOPT_ERRORBUFFER, impl->error);
	curl_easy_setopt(easy, CURLOPT_PRIVATE, this);
#ifdef DO_PROGRESS
	if (progress_func) {
		curl_easy_setopt(easy, CURLOPT_NOPROGRESS, 0L);
		curl_easy_setopt(easy, CURLOPT_XFERINFOFUNCTION, libcurl_callbacks::on_progress);
		curl_easy_setopt(easy, CURLOPT_XFERINFODATA, impl->current_response);
	} else
#else
	{
		curl_easy_setopt(easy, CURLOPT_NOPROGRESS, 1L);
	}
#endif
	#ifdef DO_TIMEOUT
	curl_easy_setopt(easy, CURLOPT_LOW_SPEED_TIME, 3L);
	curl_easy_setopt(easy, CURLOPT_LOW_SPEED_LIMIT, 10L);
	#endif

	/* call this function to get a socket */
	curl_easy_setopt(easy, CURLOPT_OPENSOCKETFUNCTION, libcurl_callbacks::on_open_socket);
	curl_easy_setopt(easy, CURLOPT_OPENSOCKETDATA, ch);

	/* call this function to close a socket */
	curl_easy_setopt(easy, CURLOPT_CLOSESOCKETFUNCTION, libcurl_callbacks::on_close_socket);
	curl_easy_setopt(easy, CURLOPT_CLOSESOCKETDATA, ch);

	// Make sure our channel is ready to do business.
	impl->channel->open();

	auto rc = curl_multi_add_handle(ch->multi, easy);
	mcode_or_die("new_conn: multi_add_handle", rc);

	/* note that the add_handle() will set a time-out to trigger very soon so
	   that the necessary socket_action() call will be called by this app */

	return impl->current_response;
}


request session::get_current_request() {
	return impl->current_request;
}


response session::get_current_response() {
	return impl->current_response;
}


session::~session() {
	fprintf(stderr, "session %u dtor\n", get_id());
	delete impl;
}


channel * session::get_channel() {
	return impl->channel;
}


uint32_t session::get_id() const {
	return impl->id;
}


response session::get(char const * url) {
	return response(this);
}


response session::start_get(char const * url) {
	auto request = reset();
	request.set_url(url);
	request.set_verb("get");
	return send();
}


void session::merge_headers(headers & overrides) {
}


request session::reset() {
	impl->current_request->release_ref();
	impl->current_request = new request::impl_t(this);
	return request(impl->current_request);
}


#if 0

response session::get(char const * url, receive_callback rcb) {
	class request request(*this, get_http_method_by_index(http_get)->verb, url);
	merge_headers(request.get_headers());
	class response response(std::move(request), rcb);
	return response;
}

#endif

}}}} // end namespace


