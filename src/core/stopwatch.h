#ifndef intent_core_stopwatch_h
#define intent_core_stopwatch_h

#include <cstdint>

#include "chronox.h"

namespace intent {
namespace core {

/**
 * A class that is helpful for profiling code.
 */
struct stopwatch {

    typedef typename std::chrono::nanoseconds duration;
    typedef typename duration::rep rep;
    typedef typename duration::period period;
    typedef typename std::chrono::steady_clock::time_point time_point;

    uint64_t split_count;
    duration elapsed;
    time_point start_time;

    enum class state : uint8_t {
        paused,
        running
    } current_state;

    stopwatch() : split_count(0), elapsed(0), current_state(state::paused) {
    }

    struct split {
        stopwatch & watch;
        split(stopwatch & w) : watch(w) { watch.start(); }
        ~split() { watch.stop(); }
    };

    void start() {
        if (current_state == state::paused) {
            start_time = chronox::now();
            current_state = state::running;
            ++split_count;
        }
    }

    duration stop() {
        if (current_state == state::running) {
            auto n = chronox::now() - start_time;
            elapsed += n;
            current_state = state::paused;
            return n;
        }
        return duration(0);
    }

};

}} // end namespace

#endif // sentry

