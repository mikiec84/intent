#include "core/text/interp.h"
#include "lang/build_error.h"

using intent::core::text::interp;

namespace intent {
namespace lang {

build_error::build_error(char const * file, unsigned line, unsigned column,
		char const * msg): std::runtime_error(
			interp("Syntax error at {1}, line {2}, column {3}: {4}.",
				{file, line, column, msg})) {
}


} // end namespace lang
} // end namespace intent
