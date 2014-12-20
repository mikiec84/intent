#ifndef intent_core_mux_locking_strategy_h
#define intent_core_mux_locking_strategy_h

namespace intent {
namespace core {
namespace mux {

/**
 * A class that's usable in templates expecting an implementation of the Mutex
 * pattern--but that does nothing. This allows code to be written as a template,
 * and to support either thread safety (with a traditional mutex), or single
 * threaded code with no overhead.
 */
struct no_op_mutex {
};

}}} // end namespace

#endif // sentry

