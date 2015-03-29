#ifndef _864d6e90088f4c43bd37fec1d09537ca
#define _864d6e90088f4c43bd37fec1d09537ca

/**
 * Define conditions under which a timeout should occur.
 */
struct timeout {
	/** How many bytes per second is considered "too slow"? */
	unsigned bytes_per_second;
	/**
	 * How many seconds must elapse at a "too slow" transfer rate before we
	 * abandon a transfer attempt?
	 */
	unsigned seconds;

	/**
	 * Default ctor; time out after 30 secs with <= 10 bytes per sec. Same as
	 * timeout::standard.
	 */
	timeout();

	timeout(unsigned bytes_per_second, unsigned seconds);

	static const timeout standard;
	static const timeout never;
};

#include "core/net/curl/timeout-inline.h"

#endif // sentry
