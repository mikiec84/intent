#ifndef intent_core_util_already_locked_h
#define intent_core_util_already_locked_h

/**
 * In multithreaded code, it is not uncommon to run into a situation where the
 * same function needs to be called under two conditions--one where a higher-
 * level function already holds a lock, and one where it must get its own
 * lock. A common approach to this problem is the recursive mutex, but this is
 * dangerous and unnecessary. See http://j.mp/1L602gq.
 *
 * This enum provides a simple, clear way to override such functions. The
 * override that does no locking is wrapped by the one that does.
 */
enum already_locked {
	_already_locked
};


#endif // sentry
