#include "core/net/curl/timeout.h"

const timeout timeout::standard(10u, 30u);
const timeout timeout::never(0, 0);

