#ifndef _d2ec052cfec6a95117d05ca70243e693
#define _d2ec052cfec6a95117d05ca70243e693

/**
 * Use the "deprecated" macro to mark symbols as deprecated in a way that
 * the compiler recognizes. For example:
 * @code
 * deprecated("Use newer_func() instead.")
 * char const * obsolete_func() const;
 * @endcode
 */
#define deprecated(message)  __attribute__ ((deprecated(message)))

#endif // sentry
