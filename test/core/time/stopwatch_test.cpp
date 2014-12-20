#include "core/time/stopwatch.h"

#include "gtest/gtest.h"

using namespace intent::core::time;
using namespace std::chrono;

TEST(stopwatch_test, basic) {
    assignable_clock::session fake;

    stopwatch w;
    w.start();
    assignable_clock::elapse(nanoseconds(500));
    w.stop();
    for (int i = 0; i < 10; ++i) {
        stopwatch::split x(w);
        assignable_clock::elapse(milliseconds(10));
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

TEST(stopwatch_test, states_and_splits) {
    stopwatch x;
    x.start();
    EXPECT_EQ(stopwatch::state::running, x.current_state);
    // As soon as we call start, split_count should be incremented.
    EXPECT_EQ(1u, x.split_count);
    x.stop();
    EXPECT_EQ(stopwatch::state::stopped, x.current_state);
    EXPECT_EQ(1u, x.split_count);

    x.start();
    x.pause();

    // Pausing should not affect split_count.
    EXPECT_EQ(2u, x.split_count);

    // ... and neither should resuming...
    x.resume();
    EXPECT_EQ(2u, x.split_count);

    x.stop();
    EXPECT_EQ(2u, x.split_count);
}

