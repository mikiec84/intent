#ifndef _6753bbc5c9f111e480153c15c2ddfeb4
#define _6753bbc5c9f111e480153c15c2ddfeb4

// For any type that has a < operator and an == operator, generate
// >, <=, >=, and != as inline functions.
#define generate_remaining_comparison_operators(T) \
\
inline bool operator !=(T const & rhs) { \
    return !(*this == rhs); \
} \
\
inline bool operator >(T const & rhs) { \
    return rhs < *this; \
} \
\
inline bool operator <=(T const & rhs) { \
    return !(*this > rhs); \
} \
\
inline bool operator >=(T const & rhs) { \
    return !(*this < rhs); \
}

#endif // sentry
