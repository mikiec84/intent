#ifndef _dddca997c7484894bb8c67eb290d0622
#define _dddca997c7484894bb8c67eb290d0622

#include "core/marks/marks.h"

define_mark(callable_from_threads, (thread names), verb, none,
	"A useful way to make it clear from which threads we expect functionality"
	" to be invoked.")

#define callable_in_lifecycle_phases(, (phase names), verb, none,
	"Document which phases of an object or program's lifecycle provide valid"
	" context for a function call. Args are arbitrary."

#endif //sentry
