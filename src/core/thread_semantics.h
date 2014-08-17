#ifndef _thread_semantics_h_ccd186e2fcff4215a8da36fada237cdc
#define _thread_semantics_h_ccd186e2fcff4215a8da36fada237cdc

/**
 * If a class or function is decorated with this symbol, it means that you can
 * safely use it from multiple threads at the same time, without special
 * mutexing on your part. If this annotation is missing, you should assume that
 * what you want to call is NOT threadsafe.
 */
#define THREADSAFE

/**
 * Occasionally, we'll use this symbol to make it very clear that something's
 * not safe to use from multiple threads at the same time. However, unless a
 * class or function is decorated with the THREADSAFE symbol, you should assume
 * NOT_THREADSAFE anyway.
 */
#define NOT_THREADSAFE

/**
 * If a class cannot be changed after it's been constructed, mark it with this
 * symbol. This implies that it is thread-safe, and that besides the ctor
 * and static methods, it has only const methods.
 */
#define IMMUTABLE

/**
 * If a function doesn't hold, depend upon, or modify any state other than
 * what it receives from its parameters, it is "pure" (a term borrowed from
 * mathematics). Pure functions are inherently threadsafe.
 *
 * IMPORTANT CAVEAT: if the *args* passed to a PURE function are not threadsafe,
 * then the function may have side-effects, no matter how pure it is.
 */
#define PURE

/**
 * A useful way to make it clear from which threads we expect functionality
 * to be invoked. Args are arbitrary, meaningful only to developers.
 */
#define CALLABLE_FROM_THREADS(...)

#endif // sentry
