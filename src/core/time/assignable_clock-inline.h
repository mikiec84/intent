#ifndef _67539e5cc9f111e495513c15c2ddfeb4
#define _67539e5cc9f111e495513c15c2ddfeb4

#include "core/time/assignable_clock.h"

namespace intent {
namespace core {
namespace time {

inline time_point assignable_clock::now() {
    std::lock_guard<std::mutex> lock(now_mutex);
    return time_point(duration(value()));
}

template <typename D>
inline void assignable_clock::set_now(D const & val) {
    std::lock_guard<std::mutex> lock(now_mutex);
    value() = std::chrono::duration_cast<duration>(val);
}

template <>
inline void assignable_clock::set_now<assignable_clock::rep>(rep const & val) {
    std::lock_guard<std::mutex> lock(now_mutex);
    value() = duration(val);
}

template <>
inline void assignable_clock::set_now<time_point>(time_point const & val) {
    std::lock_guard<std::mutex> lock(now_mutex);
    value() = val.time_since_epoch();
}

inline void assignable_clock::elapse(duration val) {
    std::lock_guard<std::mutex> lock(now_mutex);
    value() += val;
}

inline bool assignable_clock::is_active() {
    unsigned n = session_count.load();
    // See comment in deactivate() for reason why we're also testing < 1M.
    return (n > 0 && n < 1000000);
}

inline void assignable_clock::activate() {
    ++session_count;
}

inline void assignable_clock::deactivate(bool force) {
    if (force) {
        session_count.store(0);
    } else {
        // Subtract 1 and fetch previous value, all in one atomic operation.
        unsigned n = session_count.fetch_sub(1);

        // Check for underflow. If found, undo. (Underflow can happen because
        // it's possible to forcibly deactivate the assignable_clock, cancelling all
        // open sessions. In such cases, as sessions terminate and decrement
        // session_count, underflow will happen, causing very brief instants
        // when n is near UINT_MAX. To keep these instants from causing race
        // conditions, we correct the underflow here, and we only count the
        // fake clock as active if session_count < 1M. This makes the underflow
        // harmless.)
        if (n == 0) {
            ++session_count;
        }
    }
}

inline assignable_clock::session::session() {
    assignable_clock::activate();
}

inline assignable_clock::session::session(time_point initial_fake_time) : session() {
    assignable_clock::set_now(initial_fake_time);
}

inline assignable_clock::session::~session() {
    assignable_clock::deactivate();
}

/**
 * Returns a fake timestamp when assignable_clock is active, or returns
 * std::chrono::steady_clock::now() if assignable_clock is inactive.
 */
inline time_point now() {
    if (assignable_clock::is_active()) {
        return assignable_clock::now();
    }
    return std::chrono::steady_clock::now();
}

/**
 * Behaves exactly like std::this_thread::sleep_for(), except that the interval
 * in question is simulate-able with assignable_clock.
 */
template <class R, class P>
inline void sleep_for(std::chrono::duration<R,P> const & elapsed) {
    if (assignable_clock::is_active()) {
        std::this_thread::yield();
        assignable_clock::elapse(elapsed);
    } else {
        std::this_thread::sleep_for(elapsed);
    }
}

/**
 * Behaves exactly like std::this_thread::sleep_until(), except that the interval
 * in question is simulate-able with assignable_clock.
 */
template <class C, class D>
inline void sleep_until(std::chrono::time_point<C,D> const & absolute_time) {
    if (assignable_clock::is_active()) {
        std::this_thread::yield();
        assignable_clock::set_now(absolute_time);
    } else {
        std::this_thread::sleep_until(absolute_time);
    }
}

}}} // end namespace

#endif // sentry

