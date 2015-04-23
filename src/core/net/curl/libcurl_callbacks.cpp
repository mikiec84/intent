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


/**
 * Update the event timer based on new knowledge from libcurl about next
 * time something should happen.
 */
int libcurl_callbacks::on_adjust_timeout(CURLM *multi, long timeout_ms, void * _chimpl)
{
    auto & chimpl = *reinterpret_cast<channel::impl_t *>(_chimpl);

    // Stop any pending countdown.
    chimpl.timeout.cancel();

    if (timeout_ms > 0) {

        // Setup a new countdown.
        chimpl.timeout.expires_from_now(boost::posix_time::millisec(timeout_ms));
        chimpl.timeout.async_wait(std::bind(&channel::impl_t::on_timeout, &chimpl, _1));

    } else if (timeout_ms == 0){
        // Curl needs us to call curl_multi_socket_action asap, so here goes...
        // Usually this is because a transfer is complete (succeeded or aborted).
        asio::error_code error; // success
        chimpl.on_timeout(error);

    } else {
        // Stop any pending countdown.
        chimpl.timeout.cancel();
        // timer no longer needed. Leave it cancelled.
    }

    return 0;
}


char const * get_socket_state_descrip(int state) {
    static const char * const whatstr[] = { "none", "IN", "OUT", "INOUT", "REMOVE"};
    return state > 0 && state <= 4 ? whatstr[state]: whatstr[0];
}


/**
 * CURLMOPT_SOCKETFUNCTION
 *
 * Called by curl to inform us that curl wants the status of a socket to change.
 */
int libcurl_callbacks::on_socket_update(CURL * easy, curl_socket_t sock,
        int desired_state, void * _chimpl, void * _socket_state)
{
    auto chimpl = reinterpret_cast<channel::impl_t *>(_chimpl);
    int * socket_state = reinterpret_cast<int *>(_socket_state);

    if (desired_state == CURL_POLL_REMOVE) {
        // Normally, the remote server does a graceful shutdown, and curl gives
        // us a CURLMSG_DONE telling us that our session is finished. We detect
        // that in channel::impl_t::handle_done_transfers(), and call the session's
        // cleanup_after_transfer() method. However, it is possible for a remote server to
        // close the socket prematurely, short-circuiting that code path. If
        // that happens, we can detect it here...
        session::impl_t * simpl;
        curl_easy_getinfo(easy, CURLINFO_PRIVATE, &simpl);
        if (simpl->state.load() != session_state::idle) {
            // Schedule a callback in the (very) near future to make sure we
            // end the transfer gracefully. We don't do the work right now,
            // because we would have an infinite recursion:
            // cleanup_after_transfer() -->
            //     curl_multi_remove_handle() (if we haven't removed handle) -->
            //         on_socket_update() with what==CURL_POLL_REMOVE...
            // See http://j.mp/1HMpQhz.
            chimpl->io_service.post([simpl]{ simpl->cleanup_after_transfer(); });
            chimpl->remove_socket(socket_state);
        }

    } else if (!socket_state) {
        fprintf(stderr, "Adding socket: %s\n", get_socket_state_descrip(desired_state));
        chimpl->add_socket(sock, easy, desired_state);

    } else {
        chimpl->change_socket_state(socket_state, sock, easy, desired_state);
    }

    return 0;
}


/* CURLOPT_WRITEFUNCTION */
size_t libcurl_callbacks::on_receive_data(void * data, size_t size_per_record, size_t num_records, void * _rimpl)
{
    size_t bytes_handled = 0;
    if (_rimpl) {
        auto rimpl = reinterpret_cast<response::impl_t *>(_rimpl);
        bytes_handled = rimpl->store_bytes(data, size_per_record * num_records);
    }
    return bytes_handled;
}


/* CURLOPT_WRITEHEADER */
size_t libcurl_callbacks::on_receive_header(void * data, size_t size_per_record, size_t num_records, void * _rimpl)
{
    size_t bytes_handled = 0;
    if (_rimpl) {
        auto rimpl = reinterpret_cast<response::impl_t *>(_rimpl);
        bytes_handled = rimpl->store_header(data, size_per_record * num_records);
    }
    return bytes_handled;
}


/* CURLOPT_XFERINFOFUNCTION */
int libcurl_callbacks::on_progress(void * _rimpl, uint64_t expected_receive_total,
        uint64_t received_so_far, uint64_t expected_send_total,
        uint64_t sent_so_far)
{
    int err = 0;
    if (_rimpl) {
        auto rimpl = reinterpret_cast<response::impl_t *>(_rimpl);
        if (rimpl->update_progress(expected_receive_total, received_so_far,
                 expected_send_total, sent_so_far)) {
            fprintf(stderr, "progress update: expected=%d, received=%d\n",
                    (int)expected_receive_total, (int)received_so_far);
        }

#if 0
        // We detect completion in two different ways. The normal case is that
        // received_so_far == expected_receive_total. However, sometimes curl
        // tolerates streaming downloads with no content length--in which case,
        // we see expected_receive_total == 0, and we have to test in a more
        // manual way... See http://stackoverflow.com/a/10486468.
        if (expected_receive_total) {
            if (received_so_far == expected_receive_total) {
                fprintf(stderr, "ending transfer due to progress - simple\n");
                rimpl->cleanup_after_transfer();
#if 0
                // Schedule a callback in the very near future to gracefully
                // transition us to a "completed" state.
                rimpl->session->impl->channel->impl->io_service.post([rimpl]{
                    rimpl->cleanup_after_transfer(); });
#endif
            }
        } else {
            double expected, received;
            auto rc = curl_easy_getinfo(rimpl->session->impl->easy, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &expected);
            if (rc != CURLE_OK) {
                fprintf(stderr, "curl_easy_getinfo() returned %d\n", rc);
                return err;
            }
            rc = curl_easy_getinfo(rimpl->session->impl->easy, CURLINFO_SIZE_DOWNLOAD, &received);
            if (rc != CURLE_OK) {
                fprintf(stderr, "curl_easy_getinfo() returned %d\n", rc);
                return err;
            }
            if (received == expected) {
                fprintf(stderr, "ending transfer due to progress - tricky\n");
                rimpl->cleanup_after_transfer();
            } else {
                fprintf(stderr, "Can't end transfer yet %f != %f)\n", expected, received);
            }
        }
#endif
    }
    return err;
}


/* CURLOPT_OPENSOCKETFUNCTION */
curl_socket_t libcurl_callbacks::on_open_socket(void * _chimpl, curlsocktype purpose,
                                curl_sockaddr *address)
{
    curl_socket_t sockfd = CURL_SOCKET_BAD;

    if (_chimpl) {
        auto & chimpl = *reinterpret_cast<channel::impl_t *>(_chimpl);

        if (purpose == CURLSOCKTYPE_IPCXN && address->family == AF_INET) {
            /* create a tcp socket object */
            asio_socket_t *tcp_socket = new asio_socket_t(chimpl.io_service);

            /* open it and get the native handle*/
            asio::error_code ec;
            tcp_socket->open(asio::ip::tcp::v4(), ec);

            if (ec) {
                fprintf(stderr, "ERROR %d: %s. Returning CURL_SOCKET_BAD to signal error.\n", ec.value(), ec.message().c_str());
            } else {
                sockfd = tcp_socket->native_handle();
                fprintf(stderr, "Opened socket %d\n", sockfd);

                /* save it for monitoring */
                chimpl.socket_map.insert(socket_pair_t(sockfd, tcp_socket));
            }
        }
    } else {
        fprintf(stderr, "no chimpl\n");
    }

    return sockfd;
}



/* CURLOPT_CLOSESOCKETFUNCTION */
int libcurl_callbacks::on_close_socket(void * _chimpl, curl_socket_t item)
{
    if (_chimpl) {
        auto & socket_map = reinterpret_cast<channel::impl_t *>(_chimpl)->socket_map;
        auto it = socket_map.find(item);

        if (it != socket_map.end()) {
            delete it->second;
            socket_map.erase(it);
        }
    } else {
        fprintf(stderr, "no chimpl\n");
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
      fprintf(stderr, "ERROR: %s returns %s\n", where, sock);
      /* ignore this error */
      return;
    }

    fprintf(stderr, "ERROR: %s returns %s\n", where, sock);

    exit(code);
  }
}


}}}} // end namespace
