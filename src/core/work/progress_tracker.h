#ifndef _51653460a61e4b899ca8ad686caeec97
#define _51653460a61e4b899ca8ad686caeec97

#include <cstdint>

#include "core/work/work_type.h"

namespace intent {
namespace core {
namespace work {

/**
 * Track complex work to completion. Allow concurrency for updating and checking
 * progress from arbitrary threads.
 */
class progress_tracker /* +<threadsafe */ {
    struct impl_t;
    impl_t * impl;
public:
    progress_tracker(char const * description);
    ~progress_tracker();

    bool should_stop() const;
    void request_stop();

    /**
     * Tell the progress tracker that some work needs to become part
     * of the current operation.
     *
     * @param unit What type of work is it?
     * @param amount How much of this unit of work is planned?
     */
    void expect_work(work_type, double amount);

    /**
     * Report that some expected work is now done.
     * @param amount
     */
    void complete_work(work_type, double amount);

    /**
     * Report that some expected work is now deemed unnecessary.
     * @param amount
     */
    void skip_work(work_type, double amount);
};


}}} // end namespace

#endif // sentry
