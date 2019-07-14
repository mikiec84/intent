#include "core/err/circuit_breaker.h"

#include "gtest/gtest.h"

using namespace intent::core::err;

bool normal_path(unsigned & call_count, unsigned & normal_count) {
    call_count += 1;
    normal_count += 1;
    return (call_count % 500 != 0);
}

bool fallback_path(unsigned & call_count, unsigned &) {
    call_count += 1;
    return true;
}

bool all_is_well(bool b) {
    return b;
}

TEST(circuit_breaker_test, simple) {
    circuit_breaker cb;

    unsigned call_count = 0;
    unsigned normal_count = 0;
    for (unsigned i = 0; i < 1000; ++i) {
        auto func = cb.choose_path(normal_path, fallback_path);
        auto result = func(call_count, normal_count);
        cb.update(result);
    }
}
