#include "core/time/stopwatch.h"
#include "core/time/chronox.h"

#include "gtest/gtest.h"

using namespace intent::core;
using intent::core::chronox::fake_clock;
using namespace std::chrono;

TEST(stopwatch_test, basic) {
    fake_clock::session fake;

    stopwatch w;
    w.start();
    fake_clock::elapse(nanoseconds(500));
    w.stop();
    for (int i = 0; i < 10; ++i) {
        stopwatch::split x(w);
        fake_clock::elapse(milliseconds(10));
    }
    EXPECT_EQ(11u, w.split_count);
    EXPECT_TRUE(milliseconds(100) < w.elapsed);
    EXPECT_TRUE(milliseconds(101) > w.elapsed);
}

TEST(stopwatch_test, relative) {
    stopwatch w1, w2;
    char buf[64];
    for (int i = 0; i < 10; ++i) {
        stopwatch::split x(w1);
        sprintf(buf, "tHiS-~file+dOESN't,exist.%d", i);
        fopen(buf, "r");
    }
    for (int i = 0; i < 10; ++i) {
        stopwatch::split x(w2);
        strlen("abc");
    }
    EXPECT_TRUE(w1.elapsed > w2.elapsed);
}
