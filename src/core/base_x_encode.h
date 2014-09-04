#ifndef intent_core_base_x_encode_h
#define intent_core_base_x_encode_h

#include <cstddef>

namespace intent {
namespace core {
namespace text {

/**
 * Represent an arbitrary, unsigned number in an arbitrary base.
 *
 * @param n The number to represent. Must be >= 0.
 * @param buf Where to write the representation.
 * @param buf_count How many bytes can be written to buf.
 * @param digit_inventory The set of digit characters to use.
 * @param base What numeric base should we use -- base 2, base 16, base 10, base 24, base 8, etc.
 */
template <typename UNIT>
bool base_x_encode(UNIT n, char * buf, size_t buf_count, char const * digit_inventory, size_t base) {
    if (!(buf && buf_count && digit_inventory && *digit_inventory && base > 1))
        return false;

    char const * end = buf + buf_count;

    // Example: we want to base-21-encode the number 2934.

    UNIT multiple = base;
    while (n >= multiple) {
        multiple *= base;
    }
    // We're now guaranteed to be one order of magnitude too big. Reduce by one order of
    // magnitude. In our example, the rightmost digit of base-21 numbers represents how many
    // 21s are present; the next column to the left represents how many 441s (21*21) are
    // present.
    multiple /= base;
    // In our example, where the input number is 2934, multiple should equal 441 now.

    while (multiple > 1) {
        // How many times is the number divisible by the current multiple? In our example,
        // this will yield 6 the first time through the loop, since 2934 / 441 = 6 remainder 288.
        UNIT idx = n / multiple;

        // Get the digit that corresponds to this quotient.
        *buf++ = digit_inventory[idx];

        // Catch overflow.
        if (buf >= end) {
            return false;
        }

        n -= (multiple * idx);
        multiple /= base;
    };

    *buf++ = digit_inventory[n];
    if (buf < end) {
        *buf = 0;
    }
    return true;
}

}}} // end namespace

#endif // sentry
