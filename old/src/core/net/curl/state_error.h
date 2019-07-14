#ifndef _9d8d3fcf0cbf4201acbf119b65501a70
#define _9d8d3fcf0cbf4201acbf119b65501a70

#include <stdexcept>

namespace intent {
namespace core {
namespace net {
namespace curl {


/**
 * A runtime error where a curl component is used in a way that's incompatible
 * with its current state. For example, this error might be thrown if a new
 * request is issued against a session that's currently processing an old
 * request.
 */
class state_error: public std::runtime_error {
public:
	state_error(std::string const & x): std::runtime_error(x) {}
	state_error(char const * x): std::runtime_error(x) {}
};


}}}} // end namespace

#endif // sentry
