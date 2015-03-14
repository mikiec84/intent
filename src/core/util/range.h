#ifndef _8d8b8124abc048418dcea2c1617d2f29
#define _8d8b8124abc048418dcea2c1617d2f29

#include <limits>

#include "core/util/comparison_operators.h"

namespace intent {
namespace core {
namespace util {

/**
 * Describe a subset of an ordered container. For now, indexes are always
 * absolute, non-negative, and half-open. Eventually, we may genericize this.
 *
 * Ranges have value semantics. They sort first by begin index, then by end index.
 */
template<typename IDX>
struct range {
    typedef IDX index_t;

    index_t begin;
    index_t end;

    static constexpr index_t npos = std::numeric_limits<index_t>::max();

    /**
     * Construct an empty range (end = begin = 0).
     */
    range();

    /**
     * Construct a range.
     *
     * @param begin
     * @param end
     */
    range(index_t begin, index_t end);

    range(range const &) = default;
    range(range &&) = default;

    range & operator =(range const &);
    range & operator =(range &&);

    size_t length() const;


    /**
     * @return true if range is not empty.
     */
    operator bool() const;

    bool operator ==(range const & rhs) const;
    bool operator <(range const & rhs) const;

    generate_remaining_comparison_operators(range<IDX>)
};

}}} // end namespace

#include "core/util/range-inline.h"

#endif
