#ifndef _9108e4db75a94e01a7b224355309b9bf
#define _9108e4db75a94e01a7b224355309b9bf

#include <cstdint>

/**
 * Provide a tiny web server that's guaranteed to be "up" on localhost, suitable
 * for temporary use in various tests. This code simply wraps a child process
 * that runs a python script, uhttpd.py.
 */
class uhttpd {
    struct impl_t;
    impl_t * impl;

public:
    uhttpd();
    ~uhttpd();

    /**
     * Get the prefix shared by all URLs exposed by this tiny web server;
     * includes hostname and port.
     */
    char const * get_base_url() const;

    /**
     * Get the port that this server instance auto-selected on startup.
     */
    uint16_t get_port() const;

    bool stop();
};


#endif // sentry
