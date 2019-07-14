#ifndef _8f400ee4e1274e7896df6c316c7c4907
#define _8f400ee4e1274e7896df6c316c7c4907

#include <atomic>

namespace intent {
namespace core {
namespace util {


/**
 * Provide a thread-safe, collision-free ID generation strategy for a given type.
 * @param id_type The numeric type that provides IDs.
 */
template <typename id_type>
struct monotonic_id {
    typedef id_type value_type;

    std::atomic<value_type> value;
    const value_type increment;

    /**
     * Create a new id with the specified start value and increment.
     */
    monotonic_id(value_type start=(value_type)0, value_type incr=(value_type)1) :
            value(start), increment(incr) {
    }

    /**
     * Return the next available id in the sequence, and increment the sequence
     * so a new one becomes available next time.
     */
    id_type next() {
        return value.fetch_add(increment);
    }

    /**
     * Reset the ID sequence so that the next ID to be assigned will be the
     * one specified here.
     */
    void set_next(id_type n) {
        value = n;
    }
};


}}} // end namespace


#endif // sentry
