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
    using SelectT = typename Select<condition, T, U>::type;



    // MostDerived
    template<typename List, typename T>
    struct MostDerived {
    private:
        using Candidate = typename MostDerived<typename List::Tail, T>::type;

    public:
        using type = typename Select<
                traits::ConvertibleV<typename List::Head, Candidate>,
                typename List::Head,
                Candidate
        >::type;
    };

    template<typename T>
    struct MostDerived<type_list::EmptyTypeList, T> {
        using type = T;
    };

    template<typename List, typename T>
    using MostDerivedT = typename MostDerived<List, T>::type;



    // DerivedToFront
    template<typename List>
    struct DerivedToFront {
    private:
        using MostDerivedInTail = typename MostDerived<typename List::Tail, typename List::Head>::type;

        using Tail = type_list::ReplaceFirstT<
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
    using DerivedToFrontT = typename DerivedToFront<List>::type;



    // GenScatterHierarchy
    template<typename List, template<typename> typename Unit>
    struct GenScatterHierarchy :
            public GenScatterHierarchy<typename List::Tail, Unit>,
            public Unit<typename List::Head> {
        static_assert(type_list::SameListsV<List, type_list::EraseDuplicatesT<List>>,
                "in scatter hierarchy all types must be unique");

        using LeftBase = Unit<typename List::Head>;
        using RightBase = GenScatterHierarchy<typename List::Tail, Unit>;
    };

    template<template<typename> typename Unit>
    struct GenScatterHierarchy<type_list::EmptyTypeList, Unit> {
        using LeftBase = NullType;
        using RightBase = NullType;
    };



    // ScatterHierarchyGetType
    template<typename Hierarchy, int index>
    struct ScatterHierarchyGetType {
        using type = typename ScatterHierarchyGetType<typename Hierarchy::RightBase, index - 1>::type;
    };

    template<typename Hierarchy>
    struct ScatterHierarchyGetType<Hierarchy, 0> {
        using type = typename Hierarchy::LeftBase;
    };

    template<int index>
    struct ScatterHierarchyGetType<NullType, index> {
        using type = NullType;
    };

    template<>
    struct ScatterHierarchyGetType<NullType, 0> {
        using type = NullType;
    };

    template<typename Hierarchy, int index>
    using ScatterHierarchyGetTypeT = typename ScatterHierarchyGetType<Hierarchy, index>::type;



    // ScatterHierarchyGet
    template<int index, typename Hierarchy>
    auto& ScatterHierarchyGet(Hierarchy &hierarchy) {
        return static_cast<
                ScatterHierarchyGetTypeT<traits::RemoveReferenceT<decltype(hierarchy)>, index>&
                >(hierarchy);
    }



    // GenLinearHierarchy
    template<typename List, template<typename, typename> typename Unit>
    struct GenLinearHierarchy
            : public Unit<typename List::Head, GenLinearHierarchy<typename List::Tail, Unit>> {
        using Base = Unit<typename List::Head, GenLinearHierarchy<typename List::Tail, Unit>>;
    };

    template<template<typename, typename> typename Unit>
    struct GenLinearHierarchy<type_list::EmptyTypeList, Unit> :
            public NullType {
        using Base = NullType;
    };



    // LinearHierarchyGetType
    template<typename Hierarchy, int index>
    struct LinearHierarchyGetType {
        using type = typename LinearHierarchyGetType<typename Hierarchy::Base, index - 1>::type;
    };

    template<typename Hierarchy>
    struct LinearHierarchyGetType<Hierarchy, 0> {
        using type = typename Hierarchy::Base;
    };

    template<int index>
    struct LinearHierarchyGetType<NullType, index> {
        using type = NullType;
    };

    template<>
    struct LinearHierarchyGetType<NullType, 0> {
        using type = NullType;
    };

    template<typename Hierarchy, int index>
    using LinearHierarchyGetTypeT = typename LinearHierarchyGetType<Hierarchy, index>::type;



    // LinearHierarchyGet
    template<int index, typename Hierarchy>
    auto& LinearHierarchyGet(Hierarchy &hierarchy) {
        return static_cast<
                LinearHierarchyGetTypeT<traits::RemoveReferenceT<decltype(hierarchy)>, index>&
                >(hierarchy);
    }



    // Simple Units
    template<typename T>
    struct SimpleScatterUnit {
        T value;
    };

    template<typename T, typename Base>
    struct SimpleLinearUnit : public Base {
        T value;
    };
} // namespace hierarchy