#include <functional>
#include <cassert>
#include <iostream>
#include <string>

#include "functor.h"
#include "hierarchy.h"
#include "traits.h"
#include "tuple.h"
#include "typebased.h"
#include "typelist.h"

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

    // PolymorphicType

    struct Poly {
        virtual ~Poly() = default;
    };
    class PolyDer : public Poly {};

    struct NonPoly {};

    static_assert(!traits::PolymorphicTypeV<int>);
    static_assert(!traits::PolymorphicTypeV<A>);
    static_assert(!traits::PolymorphicTypeV<A1>);
    static_assert(!traits::PolymorphicTypeV<NonPoly>);
    static_assert(traits::PolymorphicTypeV<Poly>);
    static_assert(traits::PolymorphicTypeV<PolyDer>);
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

    h::LinearHierarchyAssign<0>(lh_with_copies);
    h::LinearHierarchyAssign<0>(lh_with_copies, 6);
    assert(h::LinearHierarchyGet<0>(lh_with_copies).value == 6);

    h::LinearHierarchyAssign<0>(lh_with_copies, 7, 8.0);
    assert(h::LinearHierarchyGet<0>(lh_with_copies).value == 7);
    assert(h::LinearHierarchyGet<1>(lh_with_copies).value == 8.0);

    h::LinearHierarchyAssign<0>(lh_with_copies, 9, 10.0, 11);
    assert(h::LinearHierarchyGet<0>(lh_with_copies).value == 9);
    assert(h::LinearHierarchyGet<1>(lh_with_copies).value == 10.0);
    assert(h::LinearHierarchyGet<2>(lh_with_copies).value == 11);

    h::LinearHierarchyAssign<1>(lh_with_copies, 12.0);
    assert(h::LinearHierarchyGet<0>(lh_with_copies).value == 9);
    assert(h::LinearHierarchyGet<1>(lh_with_copies).value == 12.0);
    assert(h::LinearHierarchyGet<2>(lh_with_copies).value == 11);

    h::LinearHierarchyAssign<1>(lh_with_copies, 13.0, 14);
    assert(h::LinearHierarchyGet<0>(lh_with_copies).value == 9);
    assert(h::LinearHierarchyGet<1>(lh_with_copies).value == 13.0);
    assert(h::LinearHierarchyGet<2>(lh_with_copies).value == 14);
}

void testTuple() {
    tuple::Tuple<int, float, double> tuple;

    static_assert(
        traits::SameTypesV<traits::RemoveReferenceT<decltype(tuple::TupleGet<0>(tuple))>, int>);

    tuple::TupleAssign<0>(tuple, 0);
    assert(tuple::TupleGet<0>(tuple) == 0);

    tuple::TupleAssign<0>(tuple, 1, 2.0);
    assert(tuple::TupleGet<0>(tuple) == 1);
    assert(tuple::TupleGet<1>(tuple) == 2.0);

    tuple::TupleAssign<0>(tuple, 3, 4.0, 5.0);
    assert(tuple::TupleGet<0>(tuple) == 3);
    assert(tuple::TupleGet<1>(tuple) == 4.0);
    assert(tuple::TupleGet<2>(tuple) == 5.0);

    tuple::TupleAssign<1>(tuple, 6.0);
    assert(tuple::TupleGet<0>(tuple) == 3);
    assert(tuple::TupleGet<1>(tuple) == 6.0);
    assert(tuple::TupleGet<2>(tuple) == 5.0);

    tuple::TupleAssign<1>(tuple, 7.0, 8.0);
    assert(tuple::TupleGet<0>(tuple) == 3);
    assert(tuple::TupleGet<1>(tuple) == 7.0);
    assert(tuple::TupleGet<2>(tuple) == 8.0);
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

std::string function() {
    return "function";
}

void testFunctor() {
    std::function fun = [](int a, int b) { return a + b; };

    functor::Functor f1(fun, 1, 2);
    assert(f1() == 3);

    functor::Functor f2(fun, 3);
    assert(f2(4) == 7);

    functor::Functor f3(fun);
    assert(f3(5, 6) == 11);

    std::function fun2 = [](int a, int b, int c, int d) {
        return std::to_string(a) + " " + std::to_string(b) + " " + std::to_string(c) + " " +
               std::to_string(d);
    };

    functor::Functor f4(fun2, 1, 2, 3, 4);
    assert(f4() == "1 2 3 4");

    functor::Functor f5(fun2);
    assert(f5(1, 2, 3, 4) == "1 2 3 4");

    functor::Functor f6(fun2, 1, 2);
    assert(f6(3, 4) == "1 2 3 4");

    functor::Functor f7(function);
    assert(f7() == "function");

    functor::Functor f8([]() { return std::string("lambda"); });
    assert(f8() == "lambda");
}

int main() {
    testTraits();
    testTypeBased();

    testTypeList();
    testHierarchy();
    testTuple();

    testFunctor();

    std::cout << "executed" << std::endl;

    return 0;
}
