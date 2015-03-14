#ifndef _6753a287c9f111e492b33c15c2ddfeb4
#define _6753a287c9f111e492b33c15c2ddfeb4

#include <atomic>
#include <chrono>
#include <mutex>
#include <thread>

namespace intent {
namespace core {
namespace time {

typedef std::chrono::steady_clock::time_point time_point;

/**
 * A clock class that returns whatever time it is told. This is helpful for
 * manipulating behavior in artificial ways without waiting unnecessarily--
 * during testing, for example.
 */
class assignable_clock {
public:
    typedef std::chrono::nanoseconds duration;
    typedef duration::rep rep;
    typedef duration::period period;
    typedef std::chrono::steady_clock::time_point time_point;

    static bool is_steady() { return true; }

private:
    static std::mutex now_mutex;
    static duration & value();
    static std::atomic<unsigned> session_count;

public:
    static time_point now();

    template <typename D>
    static void set_now(D const & val);

    static void elapse(duration val);

    /**
     * @return true if the fake clock, instead of the normal
     *     std::chrono::steady_clock, provides wait logic in calls to
     *     intent::core::this_thread::sleep_until() and sleep_for().
     */
    static bool is_active();
    static void activate();
    static void deactivate(bool force = false);

    /**
     * Activate assignable_clock in ctor, deactivate in dtor.
     */
    struct session {
        session();
        session(time_point initial_fake_time);
        ~session();
    };
};

/**
 * Returns an arbitrary timestamp when assignable_clock is active, or returns
 * std::chrono::steady_clock::now() if assignable_clock is inactive.
 */
time_point now();

/**
 * Behaves exactly like std::this_thread::sleep_for(), except that the interval
 * in question is simulate-able with assignable_clock.
 */
template <class R, class P>
void sleep_for(std::chrono::duration<R,P> const & elapsed);

/**
 * Behaves exactly like std::this_thread::sleep_until(), except that the interval
 * in question is simulate-able with assignable_clock.
 */
template <class C, class D>
void sleep_until (std::chrono::time_point<C,D> const & absolute_time);

}}} // end namespace

#include "core/time/assignable_clock-inline.h"

#endif // sentry

