#ifndef intent_core_net_ip_address_type_h
#define intent_core_net_ip_address_type_h

namespace intent {
namespace core {
namespace net {

/**
 * Which type of IP address are we dealing with?
 */
enum class ip_address_type : int {
    ipv4 = 2,  // same as AF_INET
    ipv6 = 30, // same as AF_INET6
};

}}} // end namespace

#endif // sentry
