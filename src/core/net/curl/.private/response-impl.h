#ifndef _e2530873fce14f25be6d86d14358524c
#define _e2530873fce14f25be6d86d14358524c

#include <atomic>
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
    session * session; // usually not owned; see session_owned_by_me
    bool session_owned_by_me; // only true if using invisible sessions
    headers headers;
    std::string received_bytes;
    size_t expected_receive_total;
    size_t sent_byte_count;
    size_t expected_send_total;
    uint16_t status_code;
    mutable std::atomic<unsigned> ref_count;

    impl_t(class session *);
    ~impl_t();

    void add_ref() const;
    void release_ref() const;

    uint64_t store_bytes(void * bytes, uint64_t byte_count);
    uint64_t store_header(void * bytes, uint64_t byte_count);
    int update_progress(uint64_t expected_receive_total,
            uint64_t received_so_far, uint64_t expected_send_total,
            uint64_t sent_so_far);
};


}}}} // end namespace


#include "core/net/curl/.private/response-impl-inline.h"


#endif
