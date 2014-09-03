#ifndef intent_core_chronox_h
#define intent_core_chronox_h

#include <atomic>
#include <chrono>
#include <mutex>
#include <thread>

namespace intent {
namespace core {
namespace chronox {

/**
 * A clock class that returns whatever time it is told. This is helpful for
 * manipulating behavior in artificial ways without waiting unnecessarily -- for
 * example, we can "wait" a certain number of milliseconds, according to this
 * clock, and not actually wait at all. Very helpful in testing, among other
 * things.
 */
class fake_clock {
public:
    typedef std::chrono::nanoseconds duration;
    typedef duration::rep rep;
    typedef duration::period period;
    typedef std::chrono::time_point<fake_clock> time_point;

    bool is_steady() { return true; }

private:
    static std::mutex now_mutex;

    static duration & value() {
        // This is threadsafe in C++11.
        static duration the_value(0);
        return the_value;
    }

    static std::atomic<bool> active;

public:
    static time_point now() {
        std::lock_guard<std::mutex> lock(now_mutex);
        return time_point(duration(value()));
    }

    static void set_now(rep val) {
        std::lock_guard<std::mutex> lock(now_mutex);
        value() = duration(val);
    }

    template <typename D>
    static void set_now(D const & val) {
        std::lock_guard<std::mutex> lock(now_mutex);
        value() = std::chrono::duration_cast<duration>(val);
    }

    static void elapse(duration val) {
        std::lock_guard<std::mutex> lock(now_mutex);
        value() += val;
    }

    /**
     * @return true if the fake clock, instead of the normal
     *     std::chrono::steady_clock, provides wait logic in calls to
     *     intent::core::this_thread::sleep_until() and sleep_for().
     */
    static bool is_active() { return active.load(); }
    void activate(bool value) { active.store(value); }
};

/**
 * Behaves exactly like std::this_thread::sleep_for(), except that the interval
 * in question is simulate-able with fake_clock.
 */
template <class R, class P>
void sleep_for(std::chrono::duration<R,P> const & elapsed) {
    if (fake_clock::is_active()) {
        std::this_thread::yield();
        fake_clock::elapse(elapsed);
    } else {
        std::this_thread::sleep_for(elapsed);
    }
}

/**
 * Behaves exactly like std::this_thread::sleep_until(), except that the interval
 * in question is simulate-able with fake_clock.
 */
template <class C, class D>
void sleep_until (std::chrono::time_point<C,D> const & absolute_time) {
    if (fake_clock::is_active()) {
        std::this_thread::yield();
        fake_clock::set_now(absolute_time);
    } else {
        std::this_thread::sleep_until(absolute_time);
    }
}

}}} // end namespace

#endif // sentry

