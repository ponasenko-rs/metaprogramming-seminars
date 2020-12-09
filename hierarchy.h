#pragma once

#include "typelist.h"
#include "traits.h"

namespace hierarchy {
    struct NullType {
    };

    template<bool condition, typename T, typename U>
    struct Select {
        using type = T;
    };

    template<typename T, typename U>
    struct Select<false, T, U> {
        using type = U;
    };

    template<bool condition, typename T, typename U>
    using Select_t = typename Select<condition, T, U>::type;



    // MostDerived
    template<typename List, typename T>
    struct MostDerived {
    private:
        using Candidate = typename MostDerived<typename List::Tail, T>::type;

    public:
        using type = typename Select<
                traits::Convertible_v<typename List::Head, Candidate>,
                typename List::Head,
                Candidate
        >::type;
    };

    template<typename T>
    struct MostDerived<type_list::EmptyTypeList, T> {
        using type = T;
    };

    template<typename List, typename T>
    using MostDerived_t = typename MostDerived<List, T>::type;


    // DerivedToFront
    template<typename List>
    struct DerivedToFront {
    private:
        using MostDerivedInTail = typename MostDerived<typename List::Tail, typename List::Head>::type;

        using Tail = type_list::ReplaceFirst_t<
                typename List::Tail,
                MostDerivedInTail,
                typename List::Head
        >;

        using Head = MostDerivedInTail;

    public:
        using type = type_list::TypeList<Head, typename DerivedToFront<Tail>::type>;
    };

    template<>
    struct DerivedToFront<type_list::EmptyTypeList> {
        using type = type_list::EmptyTypeList;
    };

    template<typename List>
    using DerivedToFront_t = typename DerivedToFront<List>::type;



    // GenScatterHierarchy
    template<typename List, template<typename> typename Unit>
    struct GenScatterHierarchy :
            public GenScatterHierarchy<typename List::Tail, Unit>,
            public Unit<typename List::Head> {
        using Left = Unit<typename List::Head>;
        using Right = GenScatterHierarchy<typename List::Tail, Unit>;
    };

    template<typename T, template<typename> typename Unit>
    struct GenScatterHierarchy<type_list::TypeList<T>, Unit> :
            public Unit<T> {
        using Left = Unit<T>;
        using Right = NullType;
    };

    template<template<typename> typename Unit>
    struct GenScatterHierarchy<type_list::NullType, Unit> : {
        using Left = NullType;
        using Right = NullType;
    };



    // todo: add LinearHierarchy


    template<typename T>
    struct Unit {
        T value;
    };

} // namespace hierarchy