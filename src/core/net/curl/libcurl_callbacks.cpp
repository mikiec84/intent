#include <functional>

#include "core/net/curl/.private/channel-impl.h"
#include "core/net/curl/.private/libcurl_callbacks.h"
#include "core/net/curl/.private/session-impl.h"
#include "core/net/curl/.private/response-impl.h"


using namespace std::placeholders;


namespace intent {
namespace core {
namespace net {
namespace curl {


/* Update the event timer after curl_multi library calls */
int libcurl_callbacks::on_change_timeout(CURLM *multi, long timeout_ms, void * _chimpl)
{
	fprintf(stdout, "\nmulti_timer_cb: timeout_ms %ld", timeout_ms);

	chimpl_t * impl = reinterpret_cast<chimpl_t *>(_chimpl);

	/* cancel running timer */
	impl->timeout.cancel();

	if (timeout_ms > 0) {
		/* update timer */
		impl->timeout.expires_from_now(boost::posix_time::millisec(timeout_ms));
		impl->timeout.async_wait(std::bind(&chimpl_t::on_timeout, impl, _1));
	} else {
		/* call timeout function immediately */
		asio::error_code error; /*success*/
		impl->on_timeout(error);
	}

	return 0;
}


/* CURLMOPT_SOCKETFUNCTION */
int libcurl_callbacks::on_socket_update(CURL * easy, curl_socket_t sock, int what, void * _chimpl, void *sockp)
{
	fprintf(stdout, "\nsock_cb: socket=%d, what=%d, sockp=%p", sock, what, sockp);

	chimpl_t * chimpl = reinterpret_cast<chimpl_t *>(_chimpl);
	int *actionp = (int *) sockp;
	const char *whatstr[] = { "none", "IN", "OUT", "INOUT", "REMOVE"};

	fprintf(stdout,
		  "\nsocket callback: sock=%d easy=%p what=%s ", sock, easy, whatstr[what]);

	if (what == CURL_POLL_REMOVE) {
		fprintf(stdout, "\n");
		chimpl->remove_socket(actionp);
	} else {
		if (!actionp) {
			fprintf(stdout, "\nAdding data: %s", whatstr[what]);
			chimpl->add_socket(sock, easy, what);
		} else {
			fprintf(stdout,
				  "\nChanging action from %s to %s",
				  whatstr[*actionp], whatstr[what]);
			chimpl->change_socket_action(actionp, sock, easy, what);
		}
	}

	return 0;
}


/* CURLOPT_WRITEFUNCTION */
size_t libcurl_callbacks::on_receive_data(void * data, size_t size_per_record, size_t num_records, void * resp)
{
	size_t bytes_handled = 0;
	if (resp) {
		auto & response = *reinterpret_cast<class response *>(resp);
		auto func = response.impl->receive_func;
		if (func) {
			bytes_handled = func(response, data, size_per_record * num_records);
		}
	}
	return bytes_handled;
}

/* CURLOPT_XFERINFOFUNCTION */
int libcurl_callbacks::on_progress(void * resp, uint64_t expected_receive_total,
		uint64_t received_so_far, uint64_t expected_send_total,
		uint64_t sent_so_far)
{
	int err = 0;
	if (resp) {
		auto & response = *reinterpret_cast<class response *>(resp);
		auto func = response.impl->progress_func;
		if (func) {
			err = func(response, expected_receive_total, received_so_far,
				 expected_send_total, sent_so_far);
		}
	}
	return err;
}


/* CURLOPT_OPENSOCKETFUNCTION */
curl_socket_t libcurl_callbacks::on_open_socket(void * _chimpl, curlsocktype purpose,
								curl_sockaddr *address)
{
	curl_socket_t sockfd = CURL_SOCKET_BAD;
	fprintf(stdout, "\nopensocket :");

	if (_chimpl) {
		auto & cdata = reinterpret_cast<chimpl_t &>(*_chimpl);
		if (purpose == CURLSOCKTYPE_IPCXN && address->family == AF_INET)
		{
			/* create a tcp socket object */
			asio_socket_t *tcp_socket = new asio_socket_t(cdata.io_service);

			/* open it and get the native handle*/
			asio::error_code ec;
			tcp_socket->open(asio::ip::tcp::v4(), ec);

			if (ec) {
				fprintf(stdout, "\nERROR %d: %s. Returning CURL_SOCKET_BAD to signal error.", ec.value(), ec.message().c_str());
			} else {
				sockfd = tcp_socket->native_handle();
				fprintf(stdout, "\nOpened socket %d", sockfd);

				/* save it for monitoring */
				cdata.socket_map.insert(socket_pair_t(sockfd, tcp_socket));
			}
		}
	}

	return sockfd;
}



/* CURLOPT_CLOSESOCKETFUNCTION */
int libcurl_callbacks::on_close_socket(void * _chimpl, curl_socket_t item)
{
	fprintf(stdout, "\nclosesocket : %d", item);

	if (_chimpl) {
		auto & socket_map = reinterpret_cast<chimpl_t *>(_chimpl)->socket_map;
		auto it = socket_map.find(item);

		if (it != socket_map.end()) {
			delete it->second;
			socket_map.erase(it);
		}
	}

	return 0;
}


/* Die if we get a bad CURLMcode somewhere */
void mcode_or_die(const char *where, CURLMcode code)
{
  if (CURLM_OK != code)
  {
	const char *sock;
	switch(code)
	{
	case CURLM_CALL_MULTI_PERFORM:
	  sock = "CURLM_CALL_MULTI_PERFORM";
	  break;
	case CURLM_BAD_HANDLE:
	  sock = "CURLM_BAD_HANDLE";
	  break;
	case CURLM_BAD_EASY_HANDLE:
	  sock = "CURLM_BAD_EASY_HANDLE";
	  break;
	case CURLM_OUT_OF_MEMORY:
	  sock = "CURLM_OUT_OF_MEMORY";
	  break;
	case CURLM_INTERNAL_ERROR:
	  sock = "CURLM_INTERNAL_ERROR";
	  break;
	case CURLM_UNKNOWN_OPTION:
	  sock = "CURLM_UNKNOWN_OPTION";
	  break;
	case CURLM_LAST:
	  sock = "CURLM_LAST";
	  break;
	default:
	  sock = "CURLM_unknown";
	  break;
	case CURLM_BAD_SOCKET:
	  sock = "CURLM_BAD_SOCKET";
	  fprintf(stdout, "\nERROR: %s returns %s", where, sock);
	  /* ignore this error */
	  return;
	}

	fprintf(stdout, "\nERROR: %s returns %s", where, sock);

	exit(code);
  }
}


}}}} // end namespace
