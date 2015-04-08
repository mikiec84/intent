#ifndef _69c081ad860b47edb70efb16e11e7caf
#define _69c081ad860b47edb70efb16e11e7caf

#include <cstdint>
#include <map>
#include <mutex>

#include "core/net/curl/session.h"
#include "core/net/curl/.private/easy.h"
#include "core/net/curl/.private/response-impl.h"


namespace intent {
namespace core {
namespace net {
namespace curl {

struct session::impl_t /* --<threadsafe */ {

    uint32_t id; // +<final
    channel * channel;
    std::mutex mtx;
    request_handle current_request;
    response_handle current_response;
    struct easy easy;
    char error[CURL_ERROR_SIZE];
    session_state state;

    impl_t(class channel *);
    ~impl_t();
};


}}}} // end namespace


#endif
