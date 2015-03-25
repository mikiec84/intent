#ifndef _ce902ad719d64789b095ba896e998e3c
#define _ce902ad719d64789b095ba896e998e3c

#define COPYABLE(x) \
    x(x const & other); \
    x & operator =(x const &)

#define NOT_COPYABLE(x) \
    x(x const & other) = delete; \
    x & operator =(x const &) = delete

#define MOVEABLE(x) \
    x(x && other); \
    x & operator =(x &&)

#define NOT_MOVEABLE(x) \
    x(x &&) = delete; \
    x & operator =(x &&) = delete

#define MOVEABLE_BUT_NOT_COPYABLE(x) \
    MOVEABLE(x); \
    NOT_COPYABLE(x)

#endif // sentry

