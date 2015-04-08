#ifndef _cde04c2b82b54983831f2f6400d83bb4
#define _cde04c2b82b54983831f2f6400d83bb4

#include "core/net/curl/request.h"
#include "core/net/curl/.private/libcurl.h"


namespace intent {
namespace core {
namespace net {
namespace curl {


class session;


struct request::impl_t {

    uint32_t id;
    session * session;
    char * verb; // own
    char * url; // own
    headers headers;

    impl_t(class session &);
    ~impl_t();

};


}}}} // end namespace


#endif
