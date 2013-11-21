#include <algorithm>
#include <mutex>
#include <string>
#include <vector>

#include <sys/types.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <ifaddrs.h>

#include "foundation/netutil.h"

using std::mutex;
using std::lock_guard;
using std::vector;
using std::string;

typedef std::vector<std::pair<string, bool>> addrs_with_ipv6;

addrs_with_ipv6 const & _raw_info() {
	static addrs_with_ipv6 the_info;
	static mutex the_mutex;
	static bool inited = false;
	if (!inited) {
		lock_guard<mutex> lock(the_mutex);
		if (!inited) {
			inited = true;

		    struct ifaddrs *myaddrs, *ifa;
		    void *in_addr;
		    char buf[64];

		    if (getifaddrs(&myaddrs) == 0) {
				for (ifa = myaddrs; ifa != NULL; ifa = ifa->ifa_next)
				{
					if (ifa->ifa_addr == NULL)
						continue;
					if (!(ifa->ifa_flags & IFF_UP))
						continue;

					bool is_ipv6 = true;
					switch (ifa->ifa_addr->sa_family)
					{
						case AF_INET:
						{
							is_ipv6 = false;
							struct sockaddr_in *s4 = (struct sockaddr_in *)ifa->ifa_addr;
							in_addr = &s4->sin_addr;
							break;
						}
						case AF_INET6:
						{
							struct sockaddr_in6 *s6 = (struct sockaddr_in6 *)ifa->ifa_addr;
							in_addr = &s6->sin6_addr;
							break;
						}
						default:
							continue;
					}

					if (inet_ntop(ifa->ifa_addr->sa_family, in_addr, buf, sizeof(buf))) {
						the_info.push_back({buf, is_ipv6});
					}
				}

				freeifaddrs(myaddrs);
		    }
		}
	}
	return the_info;
}

char const * get_local_ipaddr(unsigned i) {
	addrs_with_ipv6 const & addrs = _raw_info();
	if (static_cast<size_t>(i) < addrs.size()) {
		return addrs[i].first.c_str();
	}
	return nullptr;
}


typedef std::pair<unsigned, int> prio_pair;

bool prio_pair_more(prio_pair const & a, prio_pair const & b) {
	auto answer = a.second > b.second;
	return answer;
}

char const * get_local_ipaddr(unsigned i, ipaddr_priority_func func) {

	if (func == nullptr) {
		return get_local_ipaddr(i);
	}

	addrs_with_ipv6 const & addrs = _raw_info();

	if (i >= addrs.size()) {
		return nullptr;
	}

	std::vector<std::pair<unsigned, int>> priorities;
	for (unsigned j = 0; j < addrs.size(); ++j) {
		auto addr = addrs[j].first;
		auto prio = func(addrs[j].second, addr.c_str());
		if (prio > 0) {
			priorities.push_back({j, prio});
		}
	}

	if (i >= priorities.size()) {
		return nullptr;
	}

	std::sort(priorities.begin(), priorities.end(), prio_pair_more);
	return addrs[priorities[i].first].first.c_str();
}
