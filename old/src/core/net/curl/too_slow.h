#ifndef _864d6e90088f4c43bd37fec1d09537ca
#define _864d6e90088f4c43bd37fec1d09537ca

/**
 * Define conditions under which a transfer should be abandoned because it is
 * too slow. This is different from a classic timeout, in that it deals with
 * a threshold *rate* rather than a threshold *elapsed time*.
 */
struct too_slow {
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
	too_slow();

	too_slow(unsigned bytes_per_second, unsigned seconds);

	static const too_slow standard;
	static const too_slow never;
};

#include "core/net/curl/too_slow-inline.h"

#endif // sentry
