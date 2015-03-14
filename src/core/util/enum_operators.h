#ifndef _6753da14c9f111e48f303c15c2ddfeb4
#define _6753da14c9f111e48f303c15c2ddfeb4

#include <type_traits>

#define enable_if_has_operators(which) \
    template <typename T> \
    inline typename std::enable_if<::intent::core::util::enum_has_##which##_operators<T>::value, T>::type

namespace intent {
namespace core {
namespace util {

// Unless overridden, all enums lack bitwise operators.
template <typename T>
struct enum_has_bitwise_operators {
    static constexpr bool value = false;
};

// Unless overridden, all enums lack numeric operators.
template <typename T>
struct enum_has_numeric_operators {
    static constexpr bool value = false;
};

}}} // end namespace

// This macro must be invoked from the global namespace
#define define_bitwise_operators_for_enum(e) \
    template <> \
    struct ::intent::core::util::enum_has_bitwise_operators<e> { \
        static constexpr bool value = true; \
    }

// This macro must be invoked from the global namespace
#define define_numeric_operators_for_enum(e) \
    template <> \
    struct ::intent::core::util::enum_has_numeric_operators<e> { \
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

enable_if_has_operators(numeric)
operator -(T lhs, unsigned n) {
    return static_cast<T>(static_cast<unsigned>(lhs) - n);
}

enable_if_has_operators(numeric)
operator +(T lhs, unsigned n) {
    return static_cast<T>(static_cast<unsigned>(lhs) + n);
}

enable_if_has_operators(numeric)
& operator -=(T & lhs, unsigned n) {
    lhs = lhs - n;
    return lhs;
}

enable_if_has_operators(numeric)
& operator +=(T & lhs, unsigned n) {
    lhs = lhs + n;
    return lhs;
}

enable_if_has_operators(numeric)
& operator --(T & lhs) {
    lhs -= 1;
    return lhs;
}

enable_if_has_operators(numeric)
operator --(T & lhs, int) {
    auto tmp = lhs;
    lhs -= 1;
    return tmp;
}

enable_if_has_operators(numeric)
& operator ++(T & lhs) {
    lhs += 1;
    return lhs;
}

enable_if_has_operators(numeric)
operator ++(T & lhs, int) {
    auto tmp = lhs;
    lhs += 1;
    return tmp;
}

#undef enable_if_has_operators

#endif // sentry
