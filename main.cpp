#include <iostream>
#include <cassert>

#include "typebased.h"
#include "typelist.h"
#include "traits.h"
#include "hierarchy.h"

void testTypeList() {
    using traits::SameTypesV;
    using typelist::SameListsV;
    namespace tl = typelist;

    using List = tl::TypeList<int, float, bool>;

    // Iterating
    static_assert(SameTypesV<List::Head, int>);
    static_assert(SameTypesV<List::Tail::Head, float>);
    static_assert(SameTypesV<List::Tail::Tail::Head, bool>);
    static_assert(SameTypesV<List::Tail::Tail::Tail, tl::EmptyTypeList>);

    // Length
    static_assert(tl::LengthV<List> == 3);
    static_assert(tl::LengthV<List::Tail> == 2);
    static_assert(tl::LengthV<List::Tail::Tail> == 1);
    static_assert(tl::LengthV<List::Tail::Tail::Tail> == 0);

    // SameLists
    static_assert(SameListsV<tl::EmptyTypeList, tl::EmptyTypeList>);
    static_assert(SameListsV<List, List>);
    static_assert(SameListsV<List::Tail, List::Tail>);
    static_assert(SameListsV<List::Tail::Tail, List::Tail::Tail>);
    static_assert(SameListsV<List::Tail::Tail::Tail, List::Tail::Tail::Tail>);

    static_assert(!SameListsV<tl::TypeList<int>, tl::TypeList<float>>);
    static_assert(!SameListsV<tl::TypeList<int, float>, tl::TypeList<float>>);
    static_assert(!SameListsV<tl::TypeList<float, int>, tl::TypeList<float>>);
    static_assert(!SameListsV<tl::EmptyTypeList, tl::TypeList<float>>);

    // At
    static_assert(SameTypesV<tl::AtT<List, 0>, int>);
    static_assert(SameTypesV<tl::AtT<List, 1>, float>);
    static_assert(SameTypesV<tl::AtT<List, 2>, bool>);
    static_assert(SameTypesV<tl::AtT<List, 3>, tl::NullType>);
    static_assert(SameTypesV<tl::AtT<List, 4>, tl::NullType>);

    // IndexOf
    static_assert(tl::IndexOfV<List, int> == 0);
    static_assert(tl::IndexOfV<List, float> == 1);
    static_assert(tl::IndexOfV<List, bool> == 2);
    static_assert(tl::IndexOfV<List, double> == -1);

    // Add
    static_assert(SameListsV<tl::AddT<List, double, 0>, tl::TypeList<double, int, float, bool>>);
    static_assert(SameListsV<tl::AddT<List, double, 1>, tl::TypeList<int, double, float, bool>>);
    static_assert(SameListsV<tl::AddT<List, double, 2>, tl::TypeList<int, float, double, bool>>);
    static_assert(SameListsV<tl::AddT<List, double, 3>, tl::TypeList<int, float, bool, double>>);
    static_assert(SameListsV<tl::AddT<List, double, 4>, tl::TypeList<int, float, bool, double>>);
    static_assert(SameListsV<tl::AddT<List, double, 5>, tl::TypeList<int, float, bool, double>>);

    // RemoveFirst
    static_assert(SameListsV<tl::RemoveFirstT<List, int>, tl::TypeList<float, bool>>);
    static_assert(SameListsV<tl::RemoveFirstT<List, float>, tl::TypeList<int, bool>>);
    static_assert(SameListsV<tl::RemoveFirstT<List, bool>, tl::TypeList<int, float>>);
    static_assert(SameListsV<tl::RemoveFirstT<List, double>, tl::TypeList<int, float, bool>>);

    static_assert(SameListsV<tl::RemoveFirstT<tl::EmptyTypeList, double>, tl::EmptyTypeList>);
    static_assert(
        SameListsV<
            tl::RemoveFirstT<
                tl::RemoveFirstT<tl::RemoveFirstT<tl::TypeList<bool, bool, bool>, bool>, bool>,
                bool>,
            tl::EmptyTypeList>);
    static_assert(SameListsV<tl::RemoveFirstT<tl::TypeList<bool, int, bool, double, bool>, bool>,
                             tl::TypeList<int, bool, double, bool>>);

    // RemoveAll
    static_assert(SameListsV<tl::RemoveAllT<List, int>, tl::TypeList<float, bool>>);
    static_assert(SameListsV<tl::RemoveAllT<List, float>, tl::TypeList<int, bool>>);
    static_assert(SameListsV<tl::RemoveAllT<List, bool>, tl::TypeList<int, float>>);
    static_assert(SameListsV<tl::RemoveAllT<List, double>, tl::TypeList<int, float, bool>>);

    static_assert(SameListsV<tl::RemoveAllT<tl::EmptyTypeList, double>, tl::EmptyTypeList>);
    static_assert(
        SameListsV<tl::RemoveAllT<tl::TypeList<bool, bool, bool>, bool>, tl::EmptyTypeList>);
    static_assert(SameListsV<tl::RemoveAllT<tl::TypeList<bool, int, bool, double, bool>, bool>,
                             tl::TypeList<int, double>>);

    // EraseDuplicates
    static_assert(SameListsV<tl::EraseDuplicatesT<tl::TypeList<int>>, tl::TypeList<int>>);

    static_assert(SameListsV<tl::EraseDuplicatesT<tl::TypeList<int, int>>, tl::TypeList<int>>);

    static_assert(SameListsV<tl::EraseDuplicatesT<tl::TypeList<int, int, int>>, tl::TypeList<int>>);

    static_assert(SameListsV<tl::EraseDuplicatesT<tl::TypeList<int, float, double>>,
                             tl::TypeList<int, float, double>>);

    static_assert(SameListsV<tl::EraseDuplicatesT<tl::TypeList<int, char, int, char, float, float,
                                                               char, int, double, float, double>>,
                             tl::TypeList<int, char, float, double>>);

    // ReplaceFirst
    static_assert(
        SameListsV<tl::ReplaceFirstT<List, int, double>, tl::TypeList<double, float, bool>>);
    static_assert(
        SameListsV<tl::ReplaceFirstT<List, float, double>, tl::TypeList<int, double, bool>>);
    static_assert(
        SameListsV<tl::ReplaceFirstT<List, bool, double>, tl::TypeList<int, float, double>>);
    static_assert(
        SameListsV<tl::ReplaceFirstT<List, double, char>, tl::TypeList<int, float, bool>>);

    static_assert(
        SameListsV<tl::ReplaceFirstT<tl::EmptyTypeList, double, char>, tl::EmptyTypeList>);
    static_assert(
        SameListsV<
            tl::ReplaceFirstT<
                tl::ReplaceFirstT<tl::ReplaceFirstT<tl::TypeList<bool, bool, bool>, bool, double>,
                                  bool, double>,
                bool, double>,
            tl::TypeList<double, double, double>>);
    static_assert(
        SameListsV<tl::ReplaceFirstT<tl::TypeList<bool, int, bool, double, bool>, bool, double>,
                   tl::TypeList<double, int, bool, double, bool>>);

    // ReplaceAll
    static_assert(SameListsV<tl::ReplaceAllT<tl::TypeList<int>, int, float>, tl::TypeList<float>>);

    static_assert(SameListsV<tl::ReplaceAllT<tl::TypeList<int, int>, int, float>,
                             tl::TypeList<float, float>>);

    static_assert(SameListsV<tl::ReplaceAllT<tl::TypeList<int, int, int>, int, float>,
                             tl::TypeList<float, float, float>>);

    static_assert(SameListsV<tl::ReplaceAllT<tl::TypeList<int>, float, int>, tl::TypeList<int>>);

    static_assert(SameListsV<tl::ReplaceAllT<tl::TypeList<int, double, int>, int, float>,
                             tl::TypeList<float, double, float>>);

    static_assert(SameListsV<tl::ReplaceAllT<tl::TypeList<double, double, int>, int, float>,
                             tl::TypeList<double, double, float>>);

    static_assert(SameListsV<tl::ReplaceAllT<tl::TypeList<int, double, double>, int, float>,
                             tl::TypeList<float, double, double>>);

    static_assert(SameListsV<tl::ReplaceAllT<tl::TypeList<int>, int, int>, tl::TypeList<int>>);
}

void testTraits() {
    namespace tr = traits;

    class A {};
    class A1 : public A {};
    class A2 : public A {};
    class A11 : public A1 {};

    // SameTypes
    static_assert(tr::SameTypesV<int, int>);
    static_assert(!tr::SameTypesV<int, long>);
    static_assert(!tr::SameTypesV<long, int>);
    static_assert(!tr::SameTypesV<int, float>);
    static_assert(!tr::SameTypesV<int, bool>);
    static_assert(!tr::SameTypesV<int, char>);

    // Convertible
    static_assert(tr::ConvertibleV<int, int>);
    static_assert(tr::ConvertibleV<int, float>);
    static_assert(tr::ConvertibleV<float, int>);

    static_assert(!tr::ConvertibleV<int, typelist::EmptyTypeList>);
    static_assert(!tr::ConvertibleV<typelist::EmptyTypeList, int>);

    static_assert(tr::ConvertibleV<A1, A>);
    static_assert(!tr::ConvertibleV<A, A1>);
    static_assert(tr::ConvertibleV<A2, A>);
    static_assert(!tr::ConvertibleV<A2, A1>);

    static_assert(tr::ConvertibleV<A11, A>);
    static_assert(tr::ConvertibleV<A11, A1>);
    static_assert(!tr::ConvertibleV<A11, A2>);

    // Select
    static_assert(tr::SameTypesV<tr::SelectT<true, int, double>, int>);
    static_assert(tr::SameTypesV<tr::SelectT<false, int, double>, double>);

    // All
    static_assert(tr::AllV<true, true, true>);
    static_assert(!tr::AllV<false, true, true>);
    static_assert(!tr::AllV<true, false, true>);
    static_assert(!tr::AllV<true, true, false>);
    static_assert(!tr::AllV<false, false, true>);
    static_assert(!tr::AllV<true, false, false>);
    static_assert(!tr::AllV<false, true, false>);
    static_assert(!tr::AllV<false, false, false>);

    // Any
    static_assert(tr::AnyV<true, true, true>);
    static_assert(tr::AnyV<false, true, true>);
    static_assert(tr::AnyV<true, false, true>);
    static_assert(tr::AnyV<true, true, false>);
    static_assert(tr::AnyV<false, false, true>);
    static_assert(tr::AnyV<true, false, false>);
    static_assert(tr::AnyV<false, true, false>);
    static_assert(!tr::AnyV<false, false, false>);
}

void testHierarchy() {
    using traits::SameTypesV;
    using typelist::SameListsV;

    namespace h = hierarchy;
    namespace tl = typelist;

    class A {};
    class A1 : public A {};
    class A2 : public A {};
    class A11 : public A1 {};

    class B {};
    class B1 : public B {};
    class B2 : public B {};
    class B21 : public B2 {};

    // MostDerived
    static_assert(SameTypesV<h::MostDerivedT<tl::TypeList<A>, A>, A>);

    static_assert(SameTypesV<h::MostDerivedT<tl::TypeList<A, A1>, A>, A1>);
    static_assert(SameTypesV<h::MostDerivedT<tl::TypeList<A1, A>, A>, A1>);

    static_assert(SameTypesV<h::MostDerivedT<tl::TypeList<A, A1, A11>, A>, A11>);
    static_assert(SameTypesV<h::MostDerivedT<tl::TypeList<A1, A11, A>, A>, A11>);
    static_assert(SameTypesV<h::MostDerivedT<tl::TypeList<A11, A, A1>, A>, A11>);
    static_assert(SameTypesV<h::MostDerivedT<tl::TypeList<A, A11, A1>, A>, A11>);
    static_assert(SameTypesV<h::MostDerivedT<tl::TypeList<A1, A, A11>, A>, A11>);
    static_assert(SameTypesV<h::MostDerivedT<tl::TypeList<A11, A1, A>, A>, A11>);

    // DerivedToFront
    static_assert(SameListsV<h::DerivedToFrontT<tl::TypeList<B>>, tl::TypeList<B>>);

    static_assert(SameListsV<h::DerivedToFrontT<tl::TypeList<B, B1>>, tl::TypeList<B1, B>>);

    static_assert(SameListsV<h::DerivedToFrontT<tl::TypeList<B1, B>>, tl::TypeList<B1, B>>);

    static_assert(
        SameListsV<h::DerivedToFrontT<tl::TypeList<B, A, A1, B1>>, tl::TypeList<B1, A1, A, B>>);

    static_assert(SameListsV<h::DerivedToFrontT<tl::TypeList<B, A1, B2, A, A2, B21, B1>>,
                             tl::TypeList<B1, A1, B21, A2, A, B2, B>>);

    // GenScatterHierarchy
    using HierarchyParams = tl::TypeList<int, float, double>;

    using ScatterHierarchy = h::GenScatterHierarchy<HierarchyParams, h::SimpleScatterUnit>;
    static_assert(SameTypesV<h::ScatterHierarchyGetTypeT<ScatterHierarchy, 0>,
                             h::SimpleScatterUnit<tl::AtT<HierarchyParams, 0>>>);

    static_assert(SameTypesV<h::ScatterHierarchyGetTypeT<ScatterHierarchy, 1>,
                             h::SimpleScatterUnit<tl::AtT<HierarchyParams, 1>>>);

    static_assert(SameTypesV<h::ScatterHierarchyGetTypeT<ScatterHierarchy, 2>,
                             h::SimpleScatterUnit<tl::AtT<HierarchyParams, 2>>>);

    static_assert(SameTypesV<h::ScatterHierarchyGetTypeT<ScatterHierarchy, 3>, h::NullType>);

    // ScatterHierarchyGet
    ScatterHierarchy sh_obj;
    static_assert(
        SameTypesV<decltype(h::ScatterHierarchyGet<0>(sh_obj).value), tl::AtT<HierarchyParams, 0>>);

    static_assert(
        SameTypesV<decltype(h::ScatterHierarchyGet<1>(sh_obj).value), tl::AtT<HierarchyParams, 1>>);

    static_assert(
        SameTypesV<decltype(h::ScatterHierarchyGet<2>(sh_obj).value), tl::AtT<HierarchyParams, 2>>);

    // LinearHierarchyGet
    using LinearHierarchy = h::GenLinearHierarchy<HierarchyParams, h::SimpleLinearUnit>;
    static_assert(SameTypesV<h::LinearHierarchyGetTypeT<LinearHierarchy, 3>, h::NullType>);

    LinearHierarchy lh_obj;
    static_assert(
        SameTypesV<decltype(h::LinearHierarchyGet<0>(lh_obj).value), tl::AtT<HierarchyParams, 0>>);

    static_assert(
        SameTypesV<decltype(h::LinearHierarchyGet<1>(lh_obj).value), tl::AtT<HierarchyParams, 1>>);

    static_assert(
        SameTypesV<decltype(h::LinearHierarchyGet<2>(lh_obj).value), tl::AtT<HierarchyParams, 2>>);

    static_assert(sizeof(ScatterHierarchy) == sizeof(LinearHierarchy));

    // LinearHierarchyWithTypeCopies
    h::GenLinearHierarchy<tl::TypeList<int, float, int>, h::SimpleLinearUnit> lh_with_copies;
    h::LinearHierarchyGet<0>(lh_with_copies).value = 1;
    h::LinearHierarchyGet<2>(lh_with_copies).value = 2;
    assert(h::LinearHierarchyGet<0>(lh_with_copies).value == 1);
    assert(h::LinearHierarchyGet<2>(lh_with_copies).value == 2);
}

template <typename A, typename B, typename C, typename D, typename E>
using Expression = typebased::DisjunctionT<
    typebased::ConjunctionT<A, B>, typebased::ConjunctionT<typebased::NegationT<A>, C>,
    typebased::ConjunctionT<A, D, C>,
    typebased::ConjunctionT<typebased::NegationT<A>, typebased::NegationT<C>,
                            typebased::NegationT<E>>>;

void testTypeBased() {
    // Conditional
    using traits::SameTypesV;
    using typebased::FalseType;
    using typebased::TrueType;
    namespace tb = typebased;

    // Conditional
    static_assert(SameTypesV<tb::ConditionalT<tb::TrueType, int, double>, int>);
    static_assert(SameTypesV<tb::ConditionalT<tb::FalseType, int, double>, double>);

    // Conjunction
    static_assert(SameTypesV<tb::ConjunctionT<TrueType, TrueType, TrueType>, TrueType>);
    static_assert(SameTypesV<tb::ConjunctionT<FalseType, TrueType, TrueType>, FalseType>);
    static_assert(SameTypesV<tb::ConjunctionT<TrueType, FalseType, TrueType>, FalseType>);
    static_assert(SameTypesV<tb::ConjunctionT<TrueType, TrueType, FalseType>, FalseType>);
    static_assert(SameTypesV<tb::ConjunctionT<FalseType, FalseType, TrueType>, FalseType>);
    static_assert(SameTypesV<tb::ConjunctionT<TrueType, FalseType, FalseType>, FalseType>);
    static_assert(SameTypesV<tb::ConjunctionT<FalseType, TrueType, FalseType>, FalseType>);
    static_assert(SameTypesV<tb::ConjunctionT<FalseType, FalseType, FalseType>, FalseType>);

    // Disjunction
    static_assert(SameTypesV<tb::DisjunctionT<TrueType, TrueType, TrueType>, TrueType>);
    static_assert(SameTypesV<tb::DisjunctionT<FalseType, TrueType, TrueType>, TrueType>);
    static_assert(SameTypesV<tb::DisjunctionT<TrueType, FalseType, TrueType>, TrueType>);
    static_assert(SameTypesV<tb::DisjunctionT<TrueType, TrueType, FalseType>, TrueType>);
    static_assert(SameTypesV<tb::DisjunctionT<FalseType, FalseType, TrueType>, TrueType>);
    static_assert(SameTypesV<tb::DisjunctionT<TrueType, FalseType, FalseType>, TrueType>);
    static_assert(SameTypesV<tb::DisjunctionT<FalseType, TrueType, FalseType>, TrueType>);
    static_assert(SameTypesV<tb::DisjunctionT<FalseType, FalseType, FalseType>, FalseType>);

    // Negation
    static_assert(SameTypesV<TrueType, tb::NegationT<FalseType>>);
    static_assert(SameTypesV<FalseType, tb::NegationT<TrueType>>);

    // Expression test
    static_assert(Expression<TrueType, TrueType, FalseType, FalseType, FalseType>::value);
    static_assert(!Expression<TrueType, FalseType, FalseType, FalseType, TrueType>::value);
    static_assert(!Expression<TrueType, FalseType, FalseType, TrueType, TrueType>::value);
}

int main() {
    testTraits();
    testTypeBased();

    testTypeList();
    testHierarchy();

    std::cout << "executed" << std::endl;

    return 0;
}
