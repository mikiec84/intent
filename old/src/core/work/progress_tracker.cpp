#include <atomic>
#include <string>
#include <map>
#include <mutex>

#include "core/work/progress_tracker.h"

using std::atomic;
using std::lock_guard;
using std::mutex;
using std::string;


namespace intent {
namespace core {
namespace work {

typedef std::map<work_type, double> work_map;


struct progress_tracker::impl_t {
    string descrip;
    work_map expected_work_map;
    work_map done_work_map;
    atomic<bool> should_stop;
    class mutex mutex;

    impl_t(char const * desc) : descrip(desc), expected_work_map(),
            done_work_map(), should_stop(false) {
    }
};


progress_tracker::progress_tracker(char const * description) :
        impl(new impl_t(description)) {
}


progress_tracker::~progress_tracker() {
    delete impl;
}


bool progress_tracker::should_stop() const {
    return impl->should_stop;
}


void progress_tracker::request_stop() {
    impl->should_stop = true;
}


void progress_tracker::expect_work(work_type wt, double amount) {
    if (amount > 0.0) {
        lock_guard<mutex> lock(impl->mutex);
        if (impl->expected_work_map.count(wt) == 0) {
            impl->expected_work_map[wt] = amount;
        } else {
            impl->expected_work_map[wt] += amount;
        }
    }
}


void progress_tracker::complete_work(work_type wt, double amount) {
    if (amount > 0.0) {
        lock_guard<mutex> lock(impl->mutex);
        if (impl->done_work_map.count(wt) == 0) {
            impl->done_work_map[wt] = amount;
        } else {
            impl->done_work_map[wt] += amount;
        }
    }
}

/**
 * Report that some expected work is now deemed unnecessary.
 * @param amount
 */
void progress_tracker::skip_work(work_type wt, double amount) {
    if (amount > 0.0) {
        lock_guard<mutex> lock(impl->mutex);
        auto & m = impl->expected_work_map;
        auto i = m.find(wt);
        if (i != m.end()) {
            auto new_amount = i->second - amount;
            if (new_amount <= 0.0) {
                m.erase(i);
            } else {
                i->second = new_amount;
            }
        }
    }
}


}}} // end namespace
