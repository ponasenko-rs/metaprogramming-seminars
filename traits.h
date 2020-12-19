#pragma once

namespace traits {
// Convertible
template <typename From, typename To>
class Convertible {
    using A = char;
    using Yes = A;
    struct No {
        A field[2];
    };

    static Yes Test(const To &);

    static No Test(...);

    static From Mock();

public:
    static constexpr bool value = sizeof(Test(Mock())) == sizeof(Yes);
};

template <typename T, typename U>
static constexpr bool ConvertibleV = Convertible<T, U>::value;

// SameTypes
template <typename T, typename U>
struct SameTypes {
    static constexpr bool value = false;
};

template <typename T>
struct SameTypes<T, T> {
    static constexpr bool value = true;
};

template <typename T, typename U>
static constexpr bool SameTypesV = SameTypes<T, U>::value;

// RemoveReference
template <typename T>
struct RemoveReference {
    using type = T;
};

template <typename T>
struct RemoveReference<T &> {
    using type = T;
};

template <typename T>
struct RemoveReference<T &&> {
    using type = T;
};

template <typename T>
using RemoveReferenceT = typename RemoveReference<T>::type;

// Select
template <bool condition, typename T, typename U>
struct Select {
    using type = T;
};

template <typename T, typename U>
struct Select<false, T, U> {
    using type = U;
};

template <bool condition, typename T, typename U>
using SelectT = typename Select<condition, T, U>::type;

// All
template <bool... args>
struct All {};

template <>
struct All<> {
    static constexpr bool value = true;
};

template <bool head, bool... args>
struct All<head, args...> {
    static constexpr bool value = head && All<args...>::value;
};

template <bool... args>
static constexpr bool AllV = All<args...>::value;

// Any
template <bool... args>
struct Any {};

template <>
struct Any<> {
    static constexpr bool value = false;
};

template <bool head, bool... args>
struct Any<head, args...> {
    static constexpr bool value = head || Any<args...>::value;
};

template <bool... args>
static constexpr bool AnyV = Any<args...>::value;

}  // namespace traits
