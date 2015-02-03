#include "_curl-shared.h"

namespace intent {
namespace core {
namespace net {
namespace curl {


struct session::impl_t {

    channel & channel;
    CURL *easy;
    char *url;
    char error[CURL_ERROR_SIZE];
    map<string, request *> requests;
    mutex mtx;

    impl_t(channel & ch):
        channel(ch),
        easy(curl_easy_init()),
        url(nullptr),
        requests(),
        mtx() {

        error[0] = 0;
        easy = curl_easy_init();
        if (!easy) {
            fprintf(stdout, "\ncurl_easy_init() failed, exiting!");
            exit(2);
        }

        curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION, libcurl_callbacks::on_receive_data);
        curl_easy_setopt(easy, CURLOPT_WRITEDATA, this);
        curl_easy_setopt(easy, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(easy, CURLOPT_ERRORBUFFER, error);
        curl_easy_setopt(easy, CURLOPT_PRIVATE, this);
        if (progress_func) {
            curl_easy_setopt(easy, CURLOPT_NOPROGRESS, 0L);
            curl_easy_setopt(easy, CURLOPT_XFERINFOFUNCTION, libcurl_callbacks::on_progress);
            curl_easy_setopt(easy, CURLOPT_XFERINFODATA, this);
        }
        curl_easy_setopt(easy, CURLOPT_LOW_SPEED_TIME, 3L);
        curl_easy_setopt(easy, CURLOPT_LOW_SPEED_LIMIT, 10L);

        /* call this function to get a socket */
        curl_easy_setopt(easy, CURLOPT_OPENSOCKETFUNCTION, libcurl_callbacks::on_open_socket);
        curl_easy_setopt(easy, CURLOPT_OPENSOCKETDATA, channel.impl);

        /* call this function to close a socket */
        curl_easy_setopt(easy, CURLOPT_CLOSESOCKETFUNCTION, libcurl_callbacks::on_close_socket);
        curl_easy_setopt(easy, CURLOPT_CLOSESOCKETDATA, channel.impl);

    }
};


session::session(channel & ch) :
    impl(new impl_t(ch)) {
}


session::session() :
    session(channel::get_default()) {
}



}}}} // end namespace
