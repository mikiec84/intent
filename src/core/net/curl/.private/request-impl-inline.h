#ifndef _77730a1b28e645cd9733bb7d8ac017b8
#define _77730a1b28e645cd9733bb7d8ac017b8

#include "core/net/curl/.private/request-impl.h"


namespace intent {
namespace core {
namespace net {
namespace curl {


uint32_t get_next_request_id();


inline request::impl_t::impl_t(class session * s):
		id(get_next_request_id()),
		session(s),
		verb(nullptr),
		url(nullptr),
		headers(),
		ref_count(1) {
}


inline request::impl_t::~impl_t() {
	if (verb) {
		free(verb);
	}
	if (url) {
		free(url);
	}
}


inline void request::impl_t::add_ref() const {
	++ref_count;
}


inline void request::impl_t::release_ref() const {
	if (ref_count.fetch_sub(1) == 1) {
		delete this;
	}
}


}}}} // end namespace


#endif
