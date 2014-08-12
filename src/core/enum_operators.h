#ifndef ENUM_OPERATORS_H
#define ENUM_OPERATORS_H

#include <type_traits>

#define enable_if_has_operators(which) \
    template <typename T> \
    inline typename std::enable_if<enum_has_##which##_operators<T>::value, T>::type

// Unless overridden, all enums lack bitwise operators.
template <typename T>
struct enum_has_bitwise_operators {
    static constexpr bool value = false;
};

#define define_bitwise_operators_for_enum(e) \
    template <> \
    struct enum_has_bitwise_operators<e> { \
        static constexpr bool value = true; \
    }

enable_if_has_operators(bitwise)
operator |(T lhs, T rhs) {
    return static_cast<T>(static_cast<unsigned>(lhs) | static_cast<unsigned>(rhs));
}

enable_if_has_operators(bitwise)
operator &(T lhs, T rhs) {
    return static_cast<T>(static_cast<unsigned>(lhs) & static_cast<unsigned>(rhs));
}

enable_if_has_operators(bitwise)
operator ^(T lhs, T rhs) {
    return static_cast<T>(static_cast<unsigned>(lhs) ^ static_cast<unsigned>(rhs));
}

enable_if_has_operators(bitwise)
operator ~(T lhs) {
    return static_cast<T>(~static_cast<unsigned>(lhs));
}

enable_if_has_operators(bitwise)
& operator |=(T & lhs, T rhs) {
    lhs = lhs | rhs;
    return lhs;
}

enable_if_has_operators(bitwise)
& operator &=(T & lhs, T rhs) {
    lhs = lhs & rhs;
    return lhs;
}

enable_if_has_operators(bitwise)
& operator ^=(T & lhs, T rhs) {
    lhs = lhs ^ rhs;
    return lhs;
}

// Unless overridden, all enums lack numeric operators.
template <typename T>
struct enum_has_numeric_operators {
    static constexpr bool value = false;
};

#define define_numeric_operators_for_enum(e) \
    template <> \
    struct enum_has_numeric_operators<e> { \
        static constexpr bool value = true; \
    }

#define enable_if_numeric_operators \
    template <typename T> \
    inline typename std::enable_if<enum_has_numeric_operators<T>::value, T>::type

enable_if_numeric_operators
operator -(T lhs, unsigned n) {
    static_assert(enum_has_numeric_operators<T>::value, "operator undefined for this type");
    return static_cast<T>(static_cast<unsigned>(lhs) - n);
}

enable_if_numeric_operators
operator +(T lhs, unsigned n) {
    return static_cast<T>(static_cast<unsigned>(lhs) + n);
}

enable_if_numeric_operators
& operator -=(T & lhs, unsigned n) {
    lhs = lhs - n;
    return lhs;
}

enable_if_numeric_operators
& operator +=(T & lhs, unsigned n) {
    lhs = lhs + n;
    return lhs;
}

enable_if_numeric_operators
& operator --(T & lhs) {
    lhs -= 1;
    return lhs;
}

enable_if_numeric_operators
operator --(T & lhs, int) {
    auto tmp = lhs;
    lhs -= 1;
    return tmp;
}

enable_if_numeric_operators
& operator ++(T & lhs) {
    lhs += 1;
    return lhs;
}

enable_if_numeric_operators
operator ++(T & lhs, int) {
    auto tmp = lhs;
    lhs += 1;
    return tmp;
}

#undef enable_if_has_operators

#endif // sentry
