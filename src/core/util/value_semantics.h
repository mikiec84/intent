#ifndef _67541b63c9f111e4ba783c15c2ddfeb4
#define _67541b63c9f111e4ba783c15c2ddfeb4

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

