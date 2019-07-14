#ifndef _cde04c2b82b54983831f2f6400d83bb4
#define _cde04c2b82b54983831f2f6400d83bb4

#include <atomic>

#include "core/net/curl/request.h"
#include "core/net/curl/.private/libcurl.h"


namespace intent {
namespace core {
namespace net {
namespace curl {


class session;


struct request::impl_t {

    uint32_t id;
    session * session; // not owned
    char * verb; // own
    char * url; // own
    headers headers;
    mutable std::atomic<unsigned> ref_count;

    impl_t(class session *);
    ~impl_t();

    void add_ref() const;
    void release_ref() const;
    void set_url(char const *);
    void set_verb(char const *);
    void free_verb();
};


}}}} // end namespace


#include "core/net/curl/.private/request-impl-inline.h"


#endif
