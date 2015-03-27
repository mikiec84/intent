#ifndef _fee92706933d4b6a81dfb682897b9fca
#define _fee92706933d4b6a81dfb682897b9fca

#include "core/net/curl/timeout.h"

inline timeout::timeout() : bytes_per_second(10), seconds(30) {
}

inline timeout::timeout(unsigned bps, unsigned secs) : bytes_per_second(bps), seconds(secs) {
}


#endif // sentry
