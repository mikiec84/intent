#ifndef _4ea86e1e825c40dab2ee90573d6b4e4a
#define _4ea86e1e825c40dab2ee90573d6b4e4a

#include <algorithm>
#include "core/work/discrete_task.h"


namespace intent {
namespace core {
namespace work {


inline discrete_task::discrete_task(progress_tracker & pt, work_type wt, double amt):
    progress(&pt), wtype(wt), amount(amt) {
}


inline discrete_task::~discrete_task() {
    if (amount > 0) {
        progress->complete_work(wtype, amount);
    }
}


inline void discrete_task::adjust_amount(double delta) {
    double new_amount = std::max(amount + delta, 0.0);
    delta = new_amount - old_amount;
    progress->adjust(wtype, delta)
}


inline work_type discrete_task::get_work_type() const {
    return wtype;
}


inline double discrete_task::get_amount() const {
    return amount;
}


inline void discrete_task::set_amount(double amt) {
    double delta = amt - amount;
    adjust_amount(delta);
}


}}} // end namespace

#endif // sentry
