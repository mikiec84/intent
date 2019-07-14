#include "core/time/assignable_clock.h"

namespace intent {
namespace core {
namespace time {

std::mutex assignable_clock::now_mutex;
std::atomic<unsigned> assignable_clock::session_count(0);

assignable_clock::duration & assignable_clock::value() {
    // This is threadsafe in C++11.
    static duration the_value(0);
    return the_value;
}

}}} // end namespace


