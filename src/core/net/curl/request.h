#ifndef _4bee73c5a8e642a38e1fbb4791446ab5
#define _4bee73c5a8e642a38e1fbb4791446ab5

#include <cstdint>
#include <string>

#include "core/net/headers.h"
#include "core/net/curl/callbacks.h"
#include "core/net/curl/channel.h"
#include "core/net/curl/session.h"
#include "core/net/http_method.h"

namespace intent {
namespace core {
namespace net {
namespace curl {


/**
 * Encapsulate a single request to a remote endpoint.
 *
 * Requests are not thread-safe; you must mutex them for concurrent access to
 * a single instance from more than one thread.
 */
// -<threadsafe
class request {
    struct impl_t;
    impl_t * impl;

    friend class response;
    friend class session;

    request(session *);
    request(impl_t *);

public:
    request(request &&);
    request(request const &);
    ~request();

    session & get_session();
    session const & get_session() const;

    channel & get_channel();
    channel const & get_channel() const;

    void set_verb(char const *);
    char const * get_verb() const;

    void set_url(char const *);
    char const * get_url() const;

    uint32_t get_id() const;

    headers const & get_headers() const;
    headers & get_headers();

    // Convenience methods for extremely simple use cases where we can use the
    // default channel, and a temporary, throwaway session.
    static response get(char const * url);
    static response get(std::string const & url);
    static response start_get(char const * url);
    static response start_get(std::string const & url);
};


}}}} // end namespace


#endif // sentry
