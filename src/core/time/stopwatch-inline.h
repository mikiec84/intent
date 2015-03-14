#ifndef _04d4ebadcb714a948158fcae4cd25463
#define _04d4ebadcb714a948158fcae4cd25463

#include "core/time/stopwatch.h"
#include "core/time/assignable_clock.h"

namespace intent {
namespace core {
namespace time {

inline stopwatch::stopwatch() : split_count(0), elapsed(0), current_state(state::stopped) {
}

inline void stopwatch::start() {
    if (current_state == state::stopped) {
        start_time = now();
        current_state = state::running;
        ++split_count;
    }
}

inline void stopwatch::resume() {
    if (current_state == state::paused) {
        start_time = now();
        current_state = state::running;
    }
}

inline void stopwatch::pause() {
    if (current_state == state::running) {
        auto n = now() - start_time;
        elapsed += n;
        current_state = state::paused;
    }
}

inline stopwatch::duration stopwatch::stop() {
    switch (current_state) {
    case state::running:
        {
            auto n = now() - start_time;
            elapsed += n;
            current_state = state::stopped;
            return n;
        }
    case state::paused:
        current_state = state::stopped;
    default:
        return duration(0);
    }
}

inline stopwatch::split::split(stopwatch & w) : watch(w) {
    watch.start();
}

inline stopwatch::split::~split() {
    watch.stop();
}

}}} // end namespace

#endif // sentry

