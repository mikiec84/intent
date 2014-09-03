#ifndef intent_core_nano_timer_h
#define intent_core_nano_timer_h

#include <chrono>
#include <cstdint>

namespace intent {
namespace core {

/**
 * A class that is helpful for profiling code.
 */
template <typename CLOCK=std::chrono::steady_clock>
struct stopwatch {

    typedef typename CLOCK::duration duration;
    typedef typename duration::rep rep;
    typedef typename duration::period period;
    typedef typename CLOCK::time_point time_point;

    uint64_t split_count;
    duration elapsed;
    time_point start_time;

    enum class state_t : uint8_t {
        stopped,
        running
    } state;

    stopwatch() : split_count(0), elapsed(0), state(state_t::stopped) {
    }

    struct split {
        stopwatch & watch;
        split(stopwatch & w) : watch(w) { watch.start(); }
        ~split() { watch.stop(); }
    };

    void start() {
        if (state == state_t::stopped) {
            start_time = CLOCK::now();
            state = state_t::running;
            ++split_count;
        }
    }

    duration stop() {
        if (state == state_t::running) {
            auto n = CLOCK::now() - start_time;
            elapsed += n;
            state = state_t::stopped;
            return n;
        }
        return duration(0);
    }

};

}} // end namespace

#endif // sentry

