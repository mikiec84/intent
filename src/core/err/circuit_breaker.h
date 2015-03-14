#ifndef _675269e8c9f111e481d43c15c2ddfeb4
#define _675269e8c9f111e481d43c15c2ddfeb4

namespace intent {
namespace core {
namespace err {

/**
 * A circuit_breaker provides a fail-safe mechanism that can be used when a
 * code path or interaction fails to work as normal. It might be used, for
 * example, to use local resources when a remote service is temporarily
 * offlline.
 *
 * This class has little internal state--just enough to emulate the behavior
 * of a circuit breaker in electronics. Code that uses it should monitor a
 * function call, signal, or similar interaction (a "circuit"). Each time the
 * circuit When
 * the circuit is healthy, use normal behavior. Upon sufficient evidence that
 * the circuit is not healthy, transition to a failsafe/gracefully degraded mode.
 * Possibly test the interaction later to see if it's healthy again.
 *
 * Implements the circuit_breaker pattern described by Michael Nygard in
 * _Release It_ and discussed by Martin Fowler at http://j.mp/1w2Vun2.
 */
struct circuit_breaker {

    /**
     * Describe the three possible states of the circuit_breaker.
     */
    enum class state {
        /** Circuit is closed (healthy). */
        closed,
        /** Circuit is open (unhealthy/tripped). */
        open,
        /** Circuit is open but might be ready to close again. */
        needs_retest
    };

    state current;

    circuit_breaker(state initial_state=state::closed) : current(initial_state) {}

    /**
     * Choose which code path to execute, based on the condition of the circuit.
     */
    template <typename FUNC>
    FUNC choose_path(FUNC on_closed, FUNC on_open) {
        return current == state::closed ? on_closed : on_open;
    }

    state update(bool is_healthy) {
        if (!is_healthy) {
            current = state::open;
        }
        return current;
    }
};


}}} // end namespace

#endif // sentry
