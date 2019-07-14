#define __STDC_FORMAT_MACROS 1
#include <inttypes.h>

#include <algorithm>
#include <cstring>
#include <random>

#include "core/time/stopwatch.h"
#include "perftest/perftest.h"

using std::chrono::duration_cast;
using std::chrono::nanoseconds;

using intent::core::time::stopwatch;
using intent::core::time::now;


#define ONE_BILLION 1000000000
#define ONE_MILLION 1000000
#define ONE_THOUSAND 1000

std::vector<timed_experiment *> & timed_experiment::all() {
    static std::vector<timed_experiment *> _all;
    return _all;
}


bool reg_ex(timed_experiment * ex) {
    timed_experiment::all().push_back(ex);
    return true;
}


static inline void adjust_scale(double & value, char const *& units,
        char const * thousands_units, char const * millions_units) {
    if (value > ONE_THOUSAND) {
        if (value > ONE_MILLION) {
            value /= ONE_MILLION;
            units = millions_units;
        } else {
            value /= ONE_THOUSAND;
            units = thousands_units;
        }
    }
}


void summarize(char const * suite, char const * name, stopwatch const & sw) {

    typedef std::chrono::duration<double> exact_seconds;

    auto n = sw.elapsed.count();
    double per_call = static_cast<double>(n) / sw.split_count;

    double calls = sw.split_count;
    char const * calls_units = "";
    adjust_scale(calls, calls_units, "K", "M");

    auto calls_per_sec = ONE_BILLION / per_call;
    auto cps_units = "";
    adjust_scale(calls_per_sec, cps_units, "K", "M");

    char const * per_call_units = "nanosecs";
    adjust_scale(per_call, per_call_units, "millisecs", "microsecs");

    printf("%s :: %s\n"
           "    %.1f %s/call or %.1f%s calls/sec (%.1f%s calls in %.3f secs sans overhead)\n",
           suite,
           name,
           per_call,
           per_call_units,
           calls_per_sec,
           cps_units,
           calls,
           calls_units,
           duration_cast<exact_seconds>(sw.elapsed).count()
           );
}

struct normal_overhead {
    double nanosecs_per_full_call;
    double nanosecs_per_slim_call;

    // Provide a virtual function that is ultra light-weight. Whatever it costs
    // to call this function, we want to subtract that from the cost of our
    // experiments.
    virtual void do_nothing(uint64_t & n) {
        ++n;
        asm(""); // prevent compiler from optimizing body of function.
    }

    normal_overhead() {
        uint64_t n = 0;
        stopwatch sw;
        while (sw.elapsed.count() < 100 * ONE_MILLION) {
            stopwatch::split one_trial(sw);
            do_nothing(n);
            if (n == 0) {
                break;
            }
        }
        nanosecs_per_full_call = static_cast<double>(sw.elapsed.count() / sw.split_count);

        // Now calculate a slimmer type of overhead, where we only test sw.elapsed.count()
        // occasionally.
        stopwatch sw2;
        sw2.start();
        while (true) {
            if (sw2.split_count % 10 * ONE_THOUSAND == 0) {
                sw2.stop();
                if (sw2.elapsed.count() > 100 * ONE_MILLION) {
                    break;
                }
                sw2.start();
            } else {
                ++sw2.split_count;
            }
            do_nothing(n);
        }
        nanosecs_per_slim_call = static_cast<double>(sw2.elapsed.count() / sw2.split_count);

        printf("\n");
#ifdef DEBUG
        printf("WARNING: DEBUG BUILD. REASONING ABOUT PERFORMANCE IS IFFY AT BEST.\n\n");
#endif
        summarize("overhead", "slow functions", sw);
        summarize("overhead", "fast functions", sw2);
        printf("\n");
    }
};

normal_overhead const & get_normal_overhead() {
    static normal_overhead overhead;
    return overhead;
}

double get_normal_overhead_per_full_call() {
    return get_normal_overhead().nanosecs_per_full_call;
}

double get_normal_overhead_per_slim_call() {
    return get_normal_overhead().nanosecs_per_slim_call;
}

void timed_experiment::run() {

    stopwatch sw;

    // Run the trial for a millisecond (or one time, whichever takes longer) to
    // see what sort of timing it might have.
    while (sw.elapsed.count() < ONE_MILLION) { // 1/100th of a second
        sw.start();
        run_trial();
        sw.stop();
    }
    auto full_overhead_split_count = sw.split_count;
    uint64_t slim_overhead_split_count = 0;

    // If we still have more than 1/4 of a second to spend on the trial, run
    // some more, and minimize overhead from now on. The code to minimize
    // overhead is a bit verbose, but it improves the accuracy of our results
    // for very fast functions.

    if (sw.elapsed.count() < 750 * ONE_MILLION) {
        uint64_t divisor;
        if (sw.split_count > 100 * ONE_THOUSAND) {
            divisor = 100 * ONE_THOUSAND;
        } else if (sw.split_count > 10 * ONE_THOUSAND) {
            divisor = 10 * ONE_THOUSAND;
        } else if (sw.split_count > ONE_THOUSAND) {
            divisor = ONE_THOUSAND;
        } else if (sw.split_count > 100) {
            divisor = 100;
        } else {
            divisor = 1;
        }

        auto target_time = ONE_BILLION;
        // If we have a very fast function, run for twice as long with low
        // overhead for greater accuracy.
        if (divisor >= ONE_THOUSAND) {
            target_time *= 2;
        }

        // In this loop, we fetch the latest elapsed time, and start and stop the
        // stop watch, far less frequently than up above.
        sw.start();
        if (sw.split_count % divisor) {
            --sw.split_count; // inside loop, we'll increment this; cancel the one-time extra
        }
        while (true) {
            if (sw.split_count % divisor == 0) {
                sw.stop();
                if (sw.elapsed.count() > target_time) {
                    break;
                }
                sw.start();
            } else {
                ++sw.split_count;
            }
            run_trial();
        }
        slim_overhead_split_count = sw.split_count - full_overhead_split_count;
    }


    // We know about how expensive it is to check the clock. Scale that by how many
    // times we did so in the foregoing loop. Then subtract the overhead to get
    // the real amount of time we spent running the function.
    auto overhead = nanoseconds(static_cast<uint64_t>(
        get_normal_overhead_per_full_call() * full_overhead_split_count
        + get_normal_overhead_per_slim_call() * slim_overhead_split_count));
    sw.elapsed -= overhead;

    summarize(get_suite(), get_name(), sw);
}

struct te_reference : public timed_experiment {
    virtual char const * get_suite() const { return "reference"; }
};

/*
to do: figure out how provide setup and teardown routines in these tests,
so we can calculate the overhead for those as well. That would allow us to
subtract the memcpy time from the stuff in run_trial(). We'd also need to
generalize the routine that runs a function repeatedly and subtracts overhead,
so we can do it for the setup and teardown as well, not just run_trial().
*/

struct te_reference_sort_1000_ints : public te_reference {
    int shuffled[1000];
    int items[1000];
    te_reference_sort_1000_ints() {
        std::random_device rd;
        std::mt19937 generator(rd());
        for (int i = 0; i < 1000; ++i) {
            shuffled[i] = i;
        }
        auto begin = &shuffled[0];
        auto end = begin + 1000;
        std::shuffle(begin, end, generator);
    }
    virtual char const * get_name() const { return "sort_1000_ints"; }
    virtual void run_trial() {
        memcpy(items, shuffled, sizeof(items));
        std::sort(&items[0], &items[1000]);
    }
} glue_token_values(__ex, __LINE__); bool glue_token_values(__regd, __LINE__) = reg_ex(&glue_token_values(__ex, __LINE__));

