#ifndef intent_core_net_name_resolver_h
#define intent_core_net_name_resolver_h

#include <cstdint>
#include <netdb.h> // for struct hostent

#include "core/net/ip_address_type.h"

namespace intent {
namespace core {
namespace net {

/**
 * Wrap cross-platform, flexible, async, threadsafe DNS in a convenient interface.
 *
 * A single instance of this class is capable of managing many concurrent
 * DNS lookups, including both IPv4 and IPv6. You can use it in synchronous
 * or asynchronous mode.
 *
 * One reason why you might create multiple instances of the class is because
 * each instance is tied to a common configuration, including timeouts, DNS
 * servers to contact, fallback logic, etc. You might have one name_resolver
 * that ignores /etc/hosts entirely, and that queries a single remote bind server
 * with an aggressive timeout, using TCP only, to know very quickly and confidently
 * about failures; you might have another that consults /etc/hosts in addition
 * to three remote servers, and uses UDP, for a different behavior profile.
 */
class name_resolver
{
    struct data_t;
    data_t * data;
public:
    /**
     * Create a new name_resolver.
     *
     * @param timeout_millisecs How long should an initial request to a remote
     *     bind server wait before timing out? Subsequent requests have more
     *     aggressive timeouts, scaled linearly from this value. Any value less
     *     than 10 means use default behavior, which times out after ~4 secs.
     *     This option is not relevant if UDP is disallowed.
     * @param lookups A string of 'b' and 'f' characters that specifies the order
     *     and retries of queries to the local hosts file and remote bind server(s).
     *     For example, to query the /etc/hosts file once, followed by remote
     *     bind server(s) with up to three tries, use "fbbb". To do lookups from
     *     the hosts file only, without using the network at all, use "f". Max
     *     length = 8. Not case-sensitive. Any chars other than [bfBF] are ignored.
     * @param force_tcp Only allow communication with bind servers over TCP, not
     *     UDP. Normally, DNS on port 53 uses UDP, but the library automatically
     *     falls back to TCP if the network seems to be unreliable -- so setting
     *     this to true would unusual.
     */
    name_resolver(uint16_t timeout_millisecs = 0, char const * lookups=nullptr,
            bool force_tcp=false);
    ~name_resolver();

    /**
     * Synchronously look up a host by its name.
     *
     * @param name Name to look up, e.g., "example.com"
     * @param address_type Get ipv4 or ipv6 address(es)?
     * @param host OUT struct to populate with info. See http://j.mp/1yzJvKF for
     *     details.
     * @return true if info was found.
     */
    bool get_host_by_name(char const * name, ip_address_type address_type, struct hostent & host) const;
};

}}} // end namespace

#endif // sentry
