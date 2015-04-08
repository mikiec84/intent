#ifndef _e2530873fce14f25be6d86d14358524c
#define _e2530873fce14f25be6d86d14358524c

#include <string>

#include "core/net/curl/request.h"
#include "core/net/curl/response.h"
#include "core/net/curl/.private/libcurl.h"


namespace intent {
namespace core {
namespace net {
namespace curl {


mark(-, threadsafe)
struct response::impl_t {

    uint32_t id;
    class session * session;
    bool session_owned_by_me; // only true if using invisible sessions
    request_handle request;
    receive_callback receive_func;
    progress_callback progress_func;
    headers headers;
    std::string received_bytes;
    size_t allocated_byte_count;
    size_t received_byte_count;
    size_t expected_receive_total;
    size_t sent_byte_count;
    size_t expected_send_total;
    uint16_t status_code;

    impl_t(class session &);
    ~impl_t();

};


}}}} // end namespace


#endif
