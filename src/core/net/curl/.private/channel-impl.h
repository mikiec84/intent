#ifndef intent_core_net_curl_private_channel_impl_h
#define intent_core_net_curl_private_channel_impl_h

#include <map>
#include <mutex>
#include <thread>

#include "asio.hpp"

#include "core/net/curl/channel.h"
#include "core/net/curl/.private/curl_multi.h"
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
	curl_multi multi;
	int still_running;
	asio::io_service io_service;
	asio::deadline_timer timeout;
	socket_map_t socket_map;
	std::thread service_runner;
	asio::io_service::work work;
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

};


}}}} // end namespace


#endif
