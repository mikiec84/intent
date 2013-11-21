#ifndef _FOUNDATION_NETUTIL_H_
#define _FOUNDATION_NETUTIL_H_

#include "foundation/thread_semantics.h"

/**
 * Get the string representation of the i-th ipaddress on the local machine.
 * If i exceeds number of addresses, NULL is returned. IPv6 addresses and IPv4
 * address may be intermingled. For greater precision, see override below.
 *
 * This function is threadsafe.
 */
THREADSAFE char const * get_local_ipaddr(unsigned i);

/**
 * Define a type for functions that can filter ip addresses.
 */
typedef int (ipaddr_priority_func)(bool ipv6, char const * addr);

/**
 * Get the string representation of the i-th ipaddress on the local machine.
 * If i exceeds number of addresses, NULL is returned.
 *
 * @param ipaddr_priority_func
 *     This can be used to filter out which ip addresses you see -- for example,
 *     only those that are IPv4 and not local host. The function should assign
 *     priorities > 0 for any addresses that it likes; the higher the number,
 *     the higher the priority.
 *
 * This function is threadsafe.
 */
THREADSAFE char const * get_local_ipaddr(unsigned i, ipaddr_priority_func func);

#endif
