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
	received_bytes() {
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
}


}}}} // end namespace


#endif
