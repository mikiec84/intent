#ifndef _5182a79d693244daaf448fd5977a5f6b
#define _5182a79d693244daaf448fd5977a5f6b

#include <map>
#include <mutex>
#include <thread>

#include "asio.hpp"

#include "core/net/curl/channel.h"
#include "core/net/curl/.private/multi.h"
#include "core/net/curl/.private/libcurl_callbacks.h"


using std::map;
using std::mutex;


namespace intent {
namespace core {
namespace net {
namespace curl {


typedef asio::ip::tcp::socket asio_socket_t;
typedef map<curl_socket_t, asio_socket_t *> socket_map_t;
typedef std::pair<curl_socket_t, asio_socket_t *> socket_pair_t;


/**
 * Hold all state for a channel.
 */
struct channel::impl_t {

	uint32_t id;
	struct multi multi;
	int still_running;
	asio::io_service io_service;
	asio::deadline_timer timeout;
	socket_map_t socket_map;
	std::thread service_runner;
	asio::io_service::work work;

	// The rationale behind storing a raw session * instead of a session_handle
	// is important to understand. If we stored session_handles here, then the
	// ref count on each session would never drop below 1, and we would keep
	// old sessions around forever. By storing raw pointers, we retain the
	// ability to enumerate all our sessions, without adding an artificial ref
	// count of our own. This means sessions have to have some other owner
	// (such as a request or response, or a direct session_handle) keeping them
	// alive. As soon as external ref count drops to 0, the dtor on a session
	// ptr is called. The dtor will call us and tell us to remove it from our
	// internal list as well. Perfectly clean.
	//
	// It might seem like there's a wrinkle: what if we want to rediscover
	// sessions that are "lost" (where we've started some work but are not
	// tracking the session anymore?
	//
	// Well, this is a red herring. Any work we've started has to be represented
	// somewhere, by a request, a response, or a session. The consumer of our
	// functionality has to hold some kind of a handle for the work they're
	// responsible for. If they truly release *all* handles, they also remove
	// the work, even if it's in progress. To allow otherwise would be to let
	// people create work without consequences or accountability. Ain't gonna
	// happen.
	std::map<uint32_t, session *> sessions;

	std::mutex mtx;

	impl_t();
	~impl_t();

	void on_socket_event(asio_socket_t * tcp_socket, int action);
	void on_timeout(asio::error_code const & error);

	void add_socket(curl_socket_t sock, CURL *easy, int action);
	void change_socket_action(int *fdp, curl_socket_t sock, CURL*easy, int act);
	void check_multi_info();
	void remove_socket(int *f);

	bool is_open() const;
};


}}}} // end namespace


#endif
