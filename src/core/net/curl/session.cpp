#include <atomic>
#include <map>
#include <mutex>


#include "core/net/curl/.private/session-impl.h"
#include "core/net/curl/.private/request-impl.h"
#include "core/net/curl/channel.h"
#include "core/net/curl/request.h"
#include "core/net/curl/response.h"
#include "core/util/monotonic_id.h"


using std::atomic;
using std::lock_guard;
using std::map;
using std::mutex;


using namespace intent::core::net::curl;


static uint32_t get_next_id() {
	static intent::core::util::monotonic_id<uint32_t> the_generator;
	return the_generator.next();
}


session::impl_t::impl_t(channel_handle ch):
	id(get_next_id()),
	channel(ch),
	mtx(),
	current_request(),
	current_response(),
	easy() {
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
		current_request->impl->session.reset();
		// This may cause the request's dtor to fire, if we're the only one
		// holding a handle to the request. But it's also possible that the
		// request will live on because a handle to it is also held elsewhere.
		// In fact, this is likely, since the response probably holds a handle.
		current_request.reset();
	}

	// Now do a similar release for response.
	if (current_response) {
		current_response->impl->session.reset();
		current_response.reset();
	}
}


session::session(channel_handle ch) :
		impl(new impl_t(ch)) {
	// We have to do this in the body of the ctor, because only this outer
	// class, not the impl, has the right "this" pointer.
	impl->channel->attach(this);
}


session::session() :
	session(channel::get_default()) {
}


bool session::is_open() const {
	return bool(static_cast<CURL*>(impl->easy));
}


void session::set_error(char const * txt) {
	lock_guard<mutex> lock(impl->mtx);
	impl->error = txt;
}


void session::set_error(std::string && txt) {
	lock_guard<mutex> lock(impl->mtx);
	impl->error = std::move(txt);
}


void session::set_error(std::string const & txt) {
	lock_guard<mutex> lock(impl->mtx);
	impl->error = txt;
}


bool session::open() {
	lock_guard<mutex> lock(impl->mtx);

#if 0
	if (is_open()) {
		set_error("Session is already open.");
		return false;
	}

	impl->easy = curl_easy_init();
	if (!is_open()) {
		set_error("Unable to initialze curl easy handle.");
		return false;
	}

	current_response = response_handle(new response());

	curl_easy_setopt(curl, CURLOPT_URL, request.get_url());
	set_curl_verb(curl, request.get_verb());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, libcurl_callbacks::on_receive_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error);
	curl_easy_setopt(curl, CURLOPT_PRIVATE, this);
	if (progress_func) {
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
		curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, libcurl_callbacks::on_progress);
		curl_easy_setopt(curl, CURLOPT_XFERINFODATA, this);
	}
	#ifdef DO_TIMEOUT
	curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, 3L);
	curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, 10L);
	#endif

	/* call this function to get a socket */
	curl_easy_setopt(curl, CURLOPT_OPENSOCKETFUNCTION, libcurl_callbacks::on_open_socket);
	curl_easy_setopt(curl, CURLOPT_OPENSOCKETDATA, ch->impl);

	/* call this function to close a socket */
	curl_easy_setopt(curl, CURLOPT_CLOSESOCKETFUNCTION, libcurl_callbacks::on_close_socket);
	curl_easy_setopt(curl, CURLOPT_CLOSESOCKETDATA, ch->impl);

ch->open();
fprintf(stdout,
		"\nAdding curl %p to channel %u (%s)", curl, ch->get_id(), request.get_url());
auto rc = curl_multi_add_handle(ch->impl->multi, curl);
mcode_or_die("new_conn: multi_add_handle", rc);

/* note that the add_handle() will set a time-out to trigger very soon so
   that the necessary socket_action() call will be called by this app */

#endif
return false;
}


session::~session() {
	delete impl;
}


channel_handle session::get_channel() {
	return impl->channel;
}


uint32_t session::get_id() const {
	return impl->id;
}


void session::merge_headers(headers & overrides) {
}


#if 0

response session::get(char const * url, receive_callback rcb) {
	class request request(*this, get_http_method_by_index(http_get)->verb, url);
	merge_headers(request.get_headers());
	class response response(std::move(request), rcb);
	return response;
}

#endif

