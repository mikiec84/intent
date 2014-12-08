#include <ares.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include "core/net/name_resolver.h"

namespace intent {
namespace core {
namespace net {

// Lazy-init the ares library in a thread-safe way while avoiding the
// double-checked locking antipattern. See http://en.wikipedia.org/wiki/Double-checked_locking.
struct lib_handle {
    bool ok;
    lib_handle() {
        auto status = ares_library_init(ARES_LIB_INIT_ALL);
        ok = (status == ARES_SUCCESS);
        if (!ok) {
            fprintf(stderr, "ares_library_init: %s\n", ares_strerror(status));
        }
    }
    ~lib_handle() {
        if (ok) {
            ares_library_cleanup();
        }
    }
};

static bool lib_is_available() {
    // C++11 guarantees that the constructor for this static will fire exactly
    // once, in a thread-safe manner.
    static lib_handle handle;
    return handle.ok;
}

static constexpr uint16_t MIN_VALID_TIMEOUT = 10;

struct name_resolver::data_t {
    char lookups[8];
    ares_channel channel;
    bool ok;

    data_t(uint16_t timeout_millisecs, char const * _lookups, bool force_tcp): ok(false) {
        if (!lib_is_available()) return;

        // Sanitize lookups and copy into member variable.
        bool f_specified = false;

        memset(lookups, 0, sizeof(lookups));
        if (_lookups) {
            for (unsigned i = 0, j = 0; j < sizeof(lookups) - 1 && _lookups[i]; ++i) {
                char c = tolower(_lookups[i]);
                if (c == 'b') {
                    lookups[j++] = c;
                } else if (c == 'f' && !f_specified) {
                    lookups[j++] = c;
                    f_specified = true;
                }
            }
        }

        // Guarantee at least some valid lookups if invalid input supplied.
        if (lookups[0] == 0) {
            strcpy(lookups, "fbb");
        }

        ares_options options;
        memset(&options, 0, sizeof(options));
        int optmask = 0;
        if (timeout_millisecs >= MIN_VALID_TIMEOUT) {
            optmask |= ARES_OPT_TIMEOUTMS;
            options.timeout = timeout_millisecs;
        }
        if (force_tcp) {
            optmask |= ARES_OPT_FLAGS;
            options.flags |= ARES_OPT_TIMEOUTMS;
        }

        int status = ares_init_options(&channel, &options, optmask);
        ok = (status == ARES_SUCCESS);
        if (!ok) {
            fprintf(stderr, "ares_init_options: %s\n", ares_strerror(status));
        }
    }

    ~data_t() {
        if (ok) {
            //ares_destroy_channel(channel);
        }
    }
};

name_resolver::name_resolver(uint16_t timeout_millisecs, char const * lookups,
        bool force_tcp): data(new data_t(timeout_millisecs, lookups, force_tcp)) {
}

name_resolver::~name_resolver() {
    delete data;
}

#if 0
static void
wait_ares(ares_channel channel)
{
    for(;;){
        struct timeval *tvp, tv;
        fd_set read_fds, write_fds;
        int nfds;

        FD_ZERO(&read_fds);
        FD_ZERO(&write_fds);
        nfds = ares_fds(channel, &read_fds, &write_fds);
        if(nfds == 0){
            break;
        }
        tvp = ares_timeout(channel, NULL, &tv);
        select(nfds, &read_fds, &write_fds, NULL, tvp);
        ares_process(channel, &read_fds, &write_fds);
    }
}
#endif

struct lookup_request {
    hostent & host;
    bool ok;
    lookup_request(hostent & h) : host(h), ok(false) {
    }
};

static void
callback(void *arg, int status, int timeouts, hostent * host)
{

    if(!host || status != ARES_SUCCESS){
        printf("Failed to lookup %s\n", ares_strerror(status));
        return;
    }

    printf("Found address name %s\n", host->h_name);
    char ip[INET6_ADDRSTRLEN];
    int i = 0;

    for (i = 0; host->h_addr_list[i]; ++i) {
        inet_ntop(host->h_addrtype, host->h_addr_list[i], ip, sizeof(ip));
        printf("%s\n", ip);
    }
}

bool name_resolver::get_host_by_name(const char *name, ip_address_type address_type, hostent &host) const {
    if (data->ok && name && *name) {
        lookup_request req(host);
        ares_gethostbyname(data->channel, name, static_cast<int>(address_type), callback, &req);

    }
    memset(&host, 0, sizeof(host));
    return false;
}

#if 0
static void
state_cb(void *data, int s, int read, int write)
{
    printf("Change state fd %d read:%d write:%d\n", s, read, write);
}


static void
callback(void *arg, int status, int timeouts, struct hostent *host)
{

    if(!host || status != ARES_SUCCESS){
        printf("Failed to lookup %s\n", ares_strerror(status));
        return;
    }

    printf("Found address name %s\n", host->h_name);
    char ip[INET6_ADDRSTRLEN];
    int i = 0;

    for (i = 0; host->h_addr_list[i]; ++i) {
        inet_ntop(host->h_addrtype, host->h_addr_list[i], ip, sizeof(ip));
        printf("%s\n", ip);
    }
}


int
main(void)
{
    ares_channel channel;
    int status;
    struct ares_options options;
    int optmask = 0;

    status = ares_library_init(ARES_LIB_INIT_ALL);
    if (status != ARES_SUCCESS){
        printf("ares_library_init: %s\n", ares_strerror(status));
        return 1;
    }
    //options.sock_state_cb_data;
    options.sock_state_cb = state_cb;
    optmask |= ARES_OPT_SOCK_STATE_CB;

    status = ares_init_options(&channel, &options, optmask);
    if(status != ARES_SUCCESS) {
        printf("ares_init_options: %s\n", ares_strerror(status));
        return 1;
    }

    ares_gethostbyname(channel, "google.com", AF_INET, callback, NULL);
    ares_gethostbyname(channel, "google.com", AF_INET6, callback, NULL);
    wait_ares(channel);
    ares_destroy(channel);
    ares_library_cleanup();
    printf("fin\n");
    return 0;
}

#endif

}}} // end namespace
