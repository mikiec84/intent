#include <cstdlib>

#include "core/net/curl/.private/request-impl.h"
#include "core/net/curl/.private/session-impl.h"
#include "core/net/curl/request.h"
#include "core/util/monotonic_id.h"



namespace intent {
namespace core {
namespace net {
namespace curl {


static uint32_t get_next_id() {
    static util::monotonic_id<uint32_t> the_generator;
    return the_generator.next();
}


request::impl_t::impl_t(class session & s):
        id(get_next_id()),
        session(&s),
        verb(nullptr),
        url(nullptr),
        headers(header_is_less) {
}

request::impl_t::~impl_t() {
    if (verb) {
        free(verb);
    }
    if (url) {
        free(url);
    }
}


request::request(class session & s) :
        impl(new impl_t(s)) {
    fprintf(stderr, "request %u ctor\n", get_id());
}


request::~request() {
    fprintf(stderr, "request %u dtor\n", get_id());
    delete impl;
}


void request::set_verb(char const * verb) {
    if (verb && *verb) {
        impl->verb = strdup(verb);
    }
}


char const * request::get_verb() const {
    return impl->verb;
}


void request::set_url(char const * url) {
    if (url && *url) {
        impl->url = strdup(url);
    }
}


char const * request::get_url() const {
    return impl->url;
}


channel * request::get_channel() {
    auto s = impl->session;
    return s ? s->get_channel() : nullptr;
}


uint32_t request::get_id() const {
    return impl->id;
}


headers const & request::get_headers() const {
    return impl->headers;
}


headers & request::get_headers() {
    return impl->headers;
}


response_handle request::start_get(char const * url) {
    session * s = new session();
    s->start_get(url);
    auto & resp = s->impl->current_response;
    resp->impl->session_owned_by_me = true;
    return resp;
}


response_handle request::get(char const * url) {
    auto resp = start_get(url);
    resp->wait();
    return resp;
}


}}}} // end namespace
