#ifndef _9e3b61626eb34a9590b8849996f385d2
#define _9e3b61626eb34a9590b8849996f385d2

#include "core/net/curl/.private/response-impl.h"
#include "core/net/curl/.private/session-impl.h"


namespace intent {
namespace core {
namespace net {
namespace curl {


uint32_t get_next_response_id();

inline response::impl_t::impl_t(class session * s) :
        id(get_next_response_id()),
        session(s),
        session_owned_by_me(false),
        headers(),
        received_bytes(),
        expected_receive_total(0),
        sent_byte_count(0),
        expected_send_total(0),
        status_code(0),
        effective_url(nullptr),
        ref_count(1) {
}


inline void response::impl_t::add_ref() const {
    ++ref_count;
}


inline void response::impl_t::release_ref() const {
    if (ref_count.fetch_sub(1) == 1) {
        delete this;
    }
}


inline response::impl_t::~impl_t() {
    if (session_owned_by_me) {
        delete session;
    }
    if (effective_url) {
        free(effective_url);
    }
}


inline void response::impl_t::cleanup_after_transfer() {
    // Easier to implement in session; just delegate.
    session->impl->cleanup_after_transfer();
}


}}}} // end namespace


#endif
