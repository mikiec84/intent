#ifndef _37a903d8492b489cb43953e9dbf61911
#define _37a903d8492b489cb43953e9dbf61911

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

