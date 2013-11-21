#ifndef _DOMAIN_ZMQ_HELPERS_H_
#define _DOMAIN_ZMQ_HELPERS_H_

#include "zeromq/include/zmq.h"

void zmq_close_now(void * socket);
int zmq_bind_and_log(void * socket, char const * endpoint);
int zmq_connect_and_log(void * socket, char const * endpoint);

struct zctx_closer {
	void * ctx;
	zctx_closer(void * ctx) : ctx(ctx) {}
	~zctx_closer() { if (ctx) zmq_ctx_destroy(ctx); }
};

struct zsocket_closer {
	void * socket;
	zsocket_closer(void * socket) : socket(socket) {}
	~zsocket_closer() { zmq_close_now(socket); }
};

#endif // sentry
