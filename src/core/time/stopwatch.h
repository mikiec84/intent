#ifndef _6753ae2bc9f111e49e753c15c2ddfeb4
#define _6753ae2bc9f111e49e753c15c2ddfeb4

#include <cstdint>
#include <chrono>

namespace intent {
namespace core {
namespace time {

/**
 * A class that is helpful for profiling code or for timing external processes.
 */
struct stopwatch {

    typedef typename std::chrono::nanoseconds duration;
    typedef typename duration::rep rep;
    typedef typename duration::period period;
    typedef typename std::chrono::steady_clock::time_point time_point;

    /** How many times has the stopwatch been started? */
    uint64_t split_count;

    /** What is the total elapsed time on this stopwatch? */
    duration elapsed;

    /** When did the current split begin? */
    time_point start_time;

    enum class state : uint8_t {
        /** There is no active split, and no time is elapsing. */
        stopped,
        /**
         * A split is active, but it has been temporarily suspended. This can
         * be useful if a code path needs to be timed, but some overhead in
         * the middle needs to be subtracted.
         */
        paused,
        /** A split is active, and time is elapsing. */
        running
    };

    state current_state;

    /**
     * Create a new stopwatch. Does not begin tracking elapsed time.
     */
    stopwatch();

    /**
     * Begin tracking elapsed time.
     *
     * Captures start_time and increments split_count.
     * @pre state == stopped
     */
    void start();

    /**
     * Continue tracking elapsed time after a temporary suspension. Unlike {@link #start()},
     * split_count is not incremented.
     * @pre state == paused
     */
    void resume();

    /**
     * Temporarily suspend the tracking of elapsed time.
     * @pre state == running
     */
    void pause();

    /**
     * Stop tracking time.
     *
     * @return how much time elapsed on the current split.
     * @pre state == paused || state == running
     */
    duration stop();

    /**
     * Automatically start a stopwatch when a split is created, and stop it
     * when it goes out of scope.
     */
    struct split {
        stopwatch & watch;
        split(stopwatch & w);
        ~split();
    };

};

}}} // end namespace

#include "core/time/stopwatch-inline.h"

#endif // sentry

