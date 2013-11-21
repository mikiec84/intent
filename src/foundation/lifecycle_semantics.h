#ifndef _FOUNDATION_LIFECYCLE_SEMANTICS_H_
#define _FOUNDATION_LIFECYCLE_SEMANTICS_H_

/**
 * It is common for methods to be callable only during certain phases of the
 * lifecycle of an object. These macros provide a way to document such
 * constraints. There is no associated enforcement--just the willing
 * cooperation of coders.
 */

/**
 * Document which phases of an object or program's lifecycle provide valid
 * context for a function call. Args are arbitrary.
 */
#define CALLABLE_IN_LIFECYCLE_PHASES(...)


#endif //sentry
