#pragma once

#include "traits.h"

namespace typebased {

// IntegralConstant
template <typename T, T v>
struct IntegralConstant {
    using ValueType = T;
    static constexpr ValueType value = v;

    constexpr explicit operator ValueType() const noexcept {
        return value;
    }

    constexpr ValueType operator()() const noexcept {
        return value;
    }
};

using TrueType = IntegralConstant<bool, true>;
using FalseType = IntegralConstant<bool, false>;

template <typename Condition, typename T, typename U>
struct Conditional {
    static_assert(traits::SameTypesV<Condition, TrueType> ||
                  traits::SameTypesV<Condition, FalseType>);
    using type = T;
};

template <typename T, typename U>
struct Conditional<FalseType, T, U> {
    using type = U;
};

template <typename Condition, typename T, typename U>
using ConditionalT = typename Conditional<Condition, T, U>::type;

// Conjunction
template <typename... Args>
struct Conjunction {};

template <typename T, typename... Args>
struct Conjunction<T, Args...> {
    using type = ConditionalT<T, typename Conjunction<Args...>::type, FalseType>;
};

template <>
struct Conjunction<> {
    using type = TrueType;
};

template <typename... Args>
using ConjunctionT = typename Conjunction<Args...>::type;

// Disjunction
template <typename... Args>
struct Disjunction {};

template <typename T, typename... Args>
struct Disjunction<T, Args...> {
    using type = ConditionalT<T, TrueType, typename Disjunction<Args...>::type>;
};

template <>
struct Disjunction<> {
    using type = FalseType;
};

template <typename... Args>
using DisjunctionT = typename Disjunction<Args...>::type;

// Negation
template <typename T>
struct Negation {};

template <>
struct Negation<TrueType> {
    using type = FalseType;
};

template <>
struct Negation<FalseType> {
    using type = TrueType;
};

template <typename T>
using NegationT = typename Negation<T>::type;

}  // namespace typebased
