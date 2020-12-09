#include <iostream>
#include <type_traits>

#include "hierarchy.h"
#include "typelist.h"
#include "traits.h"


void testTypeList() {
    using traits::SameTypesV;
    namespace tl = type_list;

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
    static_assert(tl::SameListsV<tl::EmptyTypeList, tl::EmptyTypeList>);
    static_assert(tl::SameListsV<List, List>);
    static_assert(tl::SameListsV<List::Tail, List::Tail>);
    static_assert(tl::SameListsV<List::Tail::Tail, List::Tail::Tail>);
    static_assert(tl::SameListsV<List::Tail::Tail::Tail, List::Tail::Tail::Tail>);

    static_assert(!tl::SameListsV<tl::TypeList<int>, tl::TypeList<float>>);
    static_assert(!tl::SameListsV<tl::TypeList<int, float>, tl::TypeList<float>>);
    static_assert(!tl::SameListsV<tl::TypeList<float, int>, tl::TypeList<float>>);
    static_assert(!tl::SameListsV<tl::EmptyTypeList, tl::TypeList<float>>);


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
    static_assert(tl::SameListsV<tl::AddT<List, double, 0>, tl::TypeList<double, int, float, bool>>);
    static_assert(tl::SameListsV<tl::AddT<List, double, 1>, tl::TypeList<int, double, float, bool>>);
    static_assert(tl::SameListsV<tl::AddT<List, double, 2>, tl::TypeList<int, float, double, bool>>);
    static_assert(tl::SameListsV<tl::AddT<List, double, 3>, tl::TypeList<int, float, bool, double>>);
    static_assert(tl::SameListsV<tl::AddT<List, double, 4>, tl::TypeList<int, float, bool, double>>);
    static_assert(tl::SameListsV<tl::AddT<List, double, 5>, tl::TypeList<int, float, bool, double>>);


    // RemoveFirst
    static_assert(tl::SameListsV<tl::RemoveFirstT<List, int>, tl::TypeList<float, bool>>);
    static_assert(tl::SameListsV<tl::RemoveFirstT<List, float>, tl::TypeList<int, bool>>);
    static_assert(tl::SameListsV<tl::RemoveFirstT<List, bool>, tl::TypeList<int, float>>);
    static_assert(tl::SameListsV<tl::RemoveFirstT<List, double>, tl::TypeList<int, float, bool>>);

    static_assert(tl::SameListsV<tl::RemoveFirstT<tl::EmptyTypeList, double>, tl::EmptyTypeList>);
    static_assert(tl::SameListsV<
            tl::RemoveFirstT<
                    tl::RemoveFirstT<
                            tl::RemoveFirstT<tl::TypeList<bool, bool, bool>, bool>,
                            bool>,
                    bool>,
            tl::EmptyTypeList
    >);
    static_assert(tl::SameListsV<
            tl::RemoveFirstT<tl::TypeList<bool, int, bool, double, bool>, bool>,
            tl::TypeList<int, bool, double, bool>
    >);


    // RemoveAll
    static_assert(tl::SameListsV<tl::RemoveAllT<List, int>, tl::TypeList<float, bool>>);
    static_assert(tl::SameListsV<tl::RemoveAllT<List, float>, tl::TypeList<int, bool>>);
    static_assert(tl::SameListsV<tl::RemoveAllT<List, bool>, tl::TypeList<int, float>>);
    static_assert(tl::SameListsV<tl::RemoveAllT<List, double>, tl::TypeList<int, float, bool>>);

    static_assert(tl::SameListsV<tl::RemoveAllT<tl::EmptyTypeList, double>, tl::EmptyTypeList>);
    static_assert(tl::SameListsV<
            tl::RemoveAllT<tl::TypeList<bool, bool, bool>, bool>,
            tl::EmptyTypeList
    >);
    static_assert(tl::SameListsV<
            tl::RemoveAllT<tl::TypeList<bool, int, bool, double, bool>, bool>,
            tl::TypeList<int, double>
    >);


    // EraseDuplicates
    static_assert(tl::SameListsV<
            tl::EraseDuplicatesT<tl::TypeList<int>>, tl::TypeList<int>
    >);

    static_assert(tl::SameListsV<
            tl::EraseDuplicatesT<tl::TypeList<int, int>>, tl::TypeList<int>
    >);

    static_assert(tl::SameListsV<
            tl::EraseDuplicatesT<tl::TypeList<int, int, int>>, tl::TypeList<int>
    >);

    static_assert(tl::SameListsV<
            tl::EraseDuplicatesT<tl::TypeList<int, char, int, char, float, float, char, int, double, float, double>>,
            tl::TypeList<int, char, float, double>
    >);

    // todo: add ReplaceFirst tests

    // ReplaceAll
    static_assert(tl::SameListsV<
            tl::ReplaceAllT<tl::TypeList<int>, int, float>, tl::TypeList<float>
    >);

    static_assert(tl::SameListsV<
            tl::ReplaceAllT<tl::TypeList<int, int>, int, float>, tl::TypeList<float, float>
    >);

    static_assert(tl::SameListsV<
            tl::ReplaceAllT<tl::TypeList<int, int, int>, int, float>, tl::TypeList<float, float, float>
    >);

    static_assert(tl::SameListsV<
            tl::ReplaceAllT<tl::TypeList<int>, float, int>, tl::TypeList<int>
    >);

    static_assert(tl::SameListsV<
            tl::ReplaceAllT<tl::TypeList<int, double, int>, int, float>, tl::TypeList<float, double, float>
    >);

    static_assert(tl::SameListsV<
            tl::ReplaceAllT<tl::TypeList<double, double, int>, int, float>, tl::TypeList<double, double, float>
    >);

    static_assert(tl::SameListsV<
            tl::ReplaceAllT<tl::TypeList<int, double, double>, int, float>, tl::TypeList<float, double, double>
    >);

    static_assert(tl::SameListsV<
            tl::ReplaceAllT<tl::TypeList<int>, int, int>, tl::TypeList<int>
    >);
}

void testTraits() {
    class A {
    };
    class A1 : public A {
    };
    class A2 : public A {
    };
    class A11 : public A1 {
    };

    // Convertible
    static_assert(traits::ConvertibleV<int, int>);
    static_assert(traits::ConvertibleV<int, float>);
    static_assert(traits::ConvertibleV<float, int>);

    static_assert(!traits::ConvertibleV<int, type_list::EmptyTypeList>);
    static_assert(!traits::ConvertibleV<type_list::EmptyTypeList, int>);

    static_assert(traits::ConvertibleV<A1, A>);
    static_assert(!traits::ConvertibleV<A, A1>);
    static_assert(traits::ConvertibleV<A2, A>);
    static_assert(!traits::ConvertibleV<A2, A1>);

    static_assert(traits::ConvertibleV<A11, A>);
    static_assert(traits::ConvertibleV<A11, A1>);
    static_assert(!traits::ConvertibleV<A11, A2>);
}


void testHierarchy() {
    using traits::SameTypesV;

    namespace h = hierarchy;
    namespace tl = type_list;

    class A {
    };
    class A1 : public A {
    };
    class A2 : public A {
    };
    class A11 : public A1 {
    };

    class B {
    };
    class B1 : public B {
    };
    class B2 : public B {
    };
    class B21 : public B2 {
    };

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
    static_assert(tl::SameListsV<
            h::DerivedToFrontT<tl::TypeList<B>>,
            tl::TypeList<B>
    >);

    static_assert(tl::SameListsV<
            h::DerivedToFrontT<tl::TypeList<B, B1>>,
            tl::TypeList<B1, B>
    >);

    static_assert(tl::SameListsV<
            h::DerivedToFrontT<tl::TypeList<B1, B>>,
            tl::TypeList<B1, B>
    >);

    static_assert(tl::SameListsV<
            h::DerivedToFrontT<tl::TypeList<B, A, A1, B1>>,
            tl::TypeList<B1, A1, A, B>
    >);

    static_assert(tl::SameListsV<
            h::DerivedToFrontT<tl::TypeList<B, A1, B2, A, A2, B21, B1>>,
            tl::TypeList<B1, A1, B21, A2, A, B2, B>
    >);

    // GenScatterHierarchy
    // todo: add tests
}

int main() {
    testTypeList();
    testTraits();
    testHierarchy();

    return 0;
}
