#include "chronox.h"

namespace intent {
namespace core {
namespace chronox {

std::mutex fake_clock::now_mutex;
std::atomic<unsigned> fake_clock::session_count(0);

fake_clock::duration & fake_clock::value() {
    // This is threadsafe in C++11.
    static duration the_value(0);
    return the_value;
}

}}} // end namespace


