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

}  // namespace traits
