#ifndef _191108cb3f024c279efea0efda826b9f
#define _191108cb3f024c279efea0efda826b9f

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
