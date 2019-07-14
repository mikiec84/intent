#ifndef _4b987afc35e64e1d93ad5daa219bdbbf
#define _4b987afc35e64e1d93ad5daa219bdbbf

/**
 * Use this macro to decorate functions, classes, methods, or variables with
 * extra semantics. Marks only have documentary utility in C++, but they are
 * highly significant in intent.
 */
#ifndef mark
	#define mark(operator, name)
#endif

/**
 * Document the semantics of a mark.
 */
#ifndef define_mark
	#define define_mark(name, params, valid_at_scope, propagation, descrip)
#endif

#endif // sentry
