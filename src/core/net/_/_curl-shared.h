#ifndef intent_core_net__curl_shared_h
#define intent_core_net__curl_shared_h

#include <thread>

#include <asio.hpp>
#define CURL_STATICLIB 1
#include <curl/curl.h>

#include "core/net/curl.h"


using asio::deadline_timer;
using asio::io_service;
using asio::error_code;

using std::lock_guard;
using std::map;
using std::mutex;
using std::string;
using std::thread;

using namespace std::placeholders;

namespace intent {
namespace core {
namespace net {
namespace curl {


typedef asio::ip::tcp::socket asio_socket_t;
typedef map<curl_socket_t, asio_socket_t *> socket_map_t;
typedef std::pair<curl_socket_t, asio_socket_t *> socket_pair_t;


void mcode_or_die(const char *where, CURLMcode code);


}}}} // end namespace

#endif // sentry
