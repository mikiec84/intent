#ifndef _2d940a24bfc04ebc8a5d51b526a593b7
#define _2d940a24bfc04ebc8a5d51b526a593b7

#include <cstdint>

#include "core/work/work_type.h"

namespace intent {
namespace core {
namespace work {

/**
 * Track a tidy, measurable subset of overall work. Guarantee that all work
 * associated with this task is accounted for.
 */
class discrete_task {
    progress_tracker * progress;
    work_type wtype;
    double amount;
public:
    /** Take responsibility for a certain type and amount of work. */
    discrete_task(progress_tracker &, work_type, double amount);

    /**
     * When task is done, any work that it still owns is automatically reported
     * as complete.
     */
    ~discrete_task();

    /**
     * Change how much work this task "owns."
     * @param delta
     */
    void adjust_amount(double delta);
};


}}} // end namespace

#endif // sentry
