#ifndef _fee92706933d4b6a81dfb682897b9fca
#define _fee92706933d4b6a81dfb682897b9fca

#include "core/net/curl/too_slow.h"

inline too_slow::too_slow() : bytes_per_second(10), seconds(30) {
}

inline too_slow::too_slow(unsigned bps, unsigned secs) : bytes_per_second(bps), seconds(secs) {
}


#endif // sentry
