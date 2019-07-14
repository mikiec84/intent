#ifndef _69c081ad860b47edb70efb16e11e7caf
#define _69c081ad860b47edb70efb16e11e7caf

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <map>
#include <mutex>

#include "core/net/curl/request.h"
#include "core/net/curl/response.h"
#include "core/net/curl/session.h"
#include "core/net/curl/.private/easy.h"


namespace intent {
namespace core {
namespace net {
namespace curl {

struct timeout;


struct session::impl_t /* --<threadsafe */ {

    uint32_t id; // +<final
    session * wrapper;
    channel * channel;
    std::mutex mtx;
    std::condition_variable state_signal;
    request::impl_t * current_request;
    response::impl_t * current_response;
    struct easy easy;
    char error[CURL_ERROR_SIZE];
    std::atomic<session_state> state;
    int curl_socket_state;

    impl_t(session *, class channel *);
    ~impl_t();

    // Implements response::wait(). See its doc comment for semantics.
    bool wait(unsigned timeout_millisecs);

    // Implements respone::impl_t::cleanup_after_transfer().
    void cleanup_after_transfer();
};


}}}} // end namespace


#include "core/net/curl/.private/session-impl-inline.h"


#endif
