#include <cstdlib>

#include "core/net/curl/.private/request-impl.h"
#include "core/net/curl/.private/response-impl.h"
#include "core/net/curl/.private/session-impl.h"
#include "core/net/curl/request.h"
#include "core/util/monotonic_id.h"



namespace intent {
namespace core {
namespace net {
namespace curl {


const char * const standard_get_verb = get_http_method_by_id(http_get)->verb;
const char * const standard_put_verb = get_http_method_by_id(http_put)->verb;
const char * const standard_post_verb = get_http_method_by_id(http_post)->verb;
const char * const standard_head_verb = get_http_method_by_id(http_head)->verb;
const char * const standard_options_verb = get_http_method_by_id(http_options)->verb;


uint32_t get_next_request_id() {
    static util::monotonic_id<uint32_t> the_generator;
    return the_generator.next();
}


request::request(session * s) :
        impl(new impl_t(s)) {
}


request::request(request && other) :
        impl(std::move(other.impl)) {
    other.impl = nullptr;
}


request::request(request const & other) :
        impl(new impl_t(other.impl->session)) {
    impl->add_ref();
}


request::request(impl_t * other) :
        impl(other) {
    other->add_ref();
}


request::~request() {
    if (impl) {
        impl->release_ref();
    }
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


session & request::get_session() {
    return *impl->session;
}


session const & request::get_session() const {
    return *impl->session;
}


channel & request::get_channel() {
    return *impl->session->impl->channel;
}


channel const & request::get_channel() const {
    return *impl->session->impl->channel;
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


response request::start_get(std::string const & url) {
    return start_get(url.c_str());
}


response request::start_get(char const * url) {
    session * s = new session();
    s->start_get(url);
    auto resp = s->impl->current_response;
    resp->session_owned_by_me = true;
    return resp;
}


response request::get(std::string const & url) {
    return request::get(url.c_str());
}


response request::get(char const * url) {
    auto resp = start_get(url);
    resp.wait();
    return resp;
}


}}}} // end namespace
