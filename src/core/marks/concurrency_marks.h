#ifndef _0d2ce6e8bdc04361b42133ca1fc380ea
#define _0d2ce6e8bdc04361b42133ca1fc380ea

#include "core/marks/marks.h"

define_mark(threadsafe, (), class | verb, codebase | down,
	"Object is safe to access concurrently from multiple threads without"
	" special mutexing."
)

define_mark(final, (), instance, lifecyle | down,
	"Can(not) be reassigned. Similar to the usage of \"final\" on variables in"
	" java (though not used in java's other sense to prevent derivation). Unlike"
	" java, +<final may be added to a variable at any point in its lifecycle to"
	" disallow assignment only from that point onward. Member variables that are"
	" declared +<final receive a single assignment only. Usually, this happens "
	" in a constructor, but it is also legal to assign in a getter with a"
	" \"once\" block, to facilitate lazy initialization."
)

define_mark(frozen, (), class | instance | method, lifecyle | down,
	"Apparent state cannot be changed by any scope where this mark obtains. Like"
	" \"const\" in C++. Classes that are +<frozen have no way to change state"
	" after they are constructed, and are therefore inherently +<threadsafe."
	" Methods that are +<frozen do not modify state; however, they may access"
	" state that some other method changes, so they provide no thread safety"
	" guarantee. Common variables, parameters, and object instances may be marked"
	" +<frozen at any point in their lifecyle, not just when declared."
)

define_mark(pure, (), verb, none,
	"If a function doesn't hold, depend upon, or modify any state other than"
	" what it receives from its args, it is \"pure\" (a term borrowed from"
	" mathematics). Pure functions are inherently threadsafe.\n"
	"\n"
	"IMPORTANT CAVEAT: if the args passed to a pure function are not"
	" threadsafe, then the function may have side-effects, no matter how"
	" pure it is.")

define_mark(callable_from_threads, (thread names), verb, none,
	"A useful way to make it clear from which threads we expect functionality"
	" to be invoked.")

define_mark(caller_must_lock, (), verb, none,
	"This function requires locking, but it expects the caller to do it.")

#endif // sentry
