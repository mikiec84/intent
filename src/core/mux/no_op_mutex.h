#ifndef _67528114c9f111e4a66b3c15c2ddfeb4
#define _67528114c9f111e4a66b3c15c2ddfeb4

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

