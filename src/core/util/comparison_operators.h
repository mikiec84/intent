#ifndef intent_core_comparison_operators_h
#define intent_core_comparison_operators_h

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
