#ifndef intent_core_chronox_h
#define intent_core_chronox_h

#include <atomic>
#include <chrono>
#include <mutex>
#include <thread>

namespace intent {
namespace core {
namespace chronox {

typedef std::chrono::steady_clock::time_point time_point;

/**
 * A clock class that returns whatever time it is told. This is helpful for
 * manipulating behavior in artificial ways without waiting unnecessarily,
 * during testing, for example.
 */
class fake_clock {
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
     * Activate fake_clock in ctor, deactivate in dtor.
     */
    struct session {
        session();
        session(time_point initial_fake_time);
        ~session();
    };
};

/**
 * Returns a fake timestamp when fake_clock is active, or returns
 * std::chrono::steady_clock::now() if fake_clock is inactive.
 */
time_point now();

/**
 * Behaves exactly like std::this_thread::sleep_for(), except that the interval
 * in question is simulate-able with fake_clock.
 */
template <class R, class P>
void sleep_for(std::chrono::duration<R,P> const & elapsed);

/**
 * Behaves exactly like std::this_thread::sleep_until(), except that the interval
 * in question is simulate-able with fake_clock.
 */
template <class C, class D>
void sleep_until (std::chrono::time_point<C,D> const & absolute_time);

}}} // end namespace

#include "core/time/chronox-inline.h"

#endif // sentry

