#include <iostream>
#include <type_traits>

#include "hierarchy.h"
#include "typelist.h"
#include "traits.h"


void testTypeList() {
    using traits::SameTypes_v;
    namespace tl = type_list;

    using List = tl::TypeList<int, float, bool>;


    // Iterating
    static_assert(SameTypes_v<List::Head, int>);
    static_assert(SameTypes_v<List::Tail::Head, float>);
    static_assert(SameTypes_v<List::Tail::Tail::Head, bool>);
    static_assert(SameTypes_v<List::Tail::Tail::Tail, tl::EmptyTypeList>);

    // Length
    static_assert(tl::Length_v<List> == 3);
    static_assert(tl::Length_v<List::Tail> == 2);
    static_assert(tl::Length_v<List::Tail::Tail> == 1);
    static_assert(tl::Length_v<List::Tail::Tail::Tail> == 0);


    // Same
    static_assert(tl::Same_v<tl::EmptyTypeList, tl::EmptyTypeList>);
    static_assert(tl::Same_v<List, List>);
    static_assert(tl::Same_v<List::Tail, List::Tail>);
    static_assert(tl::Same_v<List::Tail::Tail, List::Tail::Tail>);
    static_assert(tl::Same_v<List::Tail::Tail::Tail, List::Tail::Tail::Tail>);

    static_assert(!tl::Same_v<tl::TypeList<int>, tl::TypeList<float>>);
    static_assert(!tl::Same_v<tl::TypeList<int, float>, tl::TypeList<float>>);
    static_assert(!tl::Same_v<tl::TypeList<float, int>, tl::TypeList<float>>);
    static_assert(!tl::Same_v<tl::EmptyTypeList, tl::TypeList<float>>);


    // At
    static_assert(SameTypes_v<tl::At_t<List, 0>, int>);
    static_assert(SameTypes_v<tl::At_t<List, 1>, float>);
    static_assert(SameTypes_v<tl::At_t<List, 2>, bool>);
    static_assert(SameTypes_v<tl::At_t<List, 3>, tl::NullType>);
    static_assert(SameTypes_v<tl::At_t<List, 4>, tl::NullType>);


    // IndexOf
    static_assert(tl::IndexOf_v<List, int> == 0);
    static_assert(tl::IndexOf_v<List, float> == 1);
    static_assert(tl::IndexOf_v<List, bool> == 2);
    static_assert(tl::IndexOf_v<List, double> == -1);


    // Add
    static_assert(tl::Same_v<tl::Add_t<List, double, 0>, tl::TypeList<double, int, float, bool>>);
    static_assert(tl::Same_v<tl::Add_t<List, double, 1>, tl::TypeList<int, double, float, bool>>);
    static_assert(tl::Same_v<tl::Add_t<List, double, 2>, tl::TypeList<int, float, double, bool>>);
    static_assert(tl::Same_v<tl::Add_t<List, double, 3>, tl::TypeList<int, float, bool, double>>);
    static_assert(tl::Same_v<tl::Add_t<List, double, 4>, tl::TypeList<int, float, bool, double>>);
    static_assert(tl::Same_v<tl::Add_t<List, double, 5>, tl::TypeList<int, float, bool, double>>);


    // RemoveFirst
    static_assert(tl::Same_v<tl::RemoveFirst_t<List, int>, tl::TypeList<float, bool>>);
    static_assert(tl::Same_v<tl::RemoveFirst_t<List, float>, tl::TypeList<int, bool>>);
    static_assert(tl::Same_v<tl::RemoveFirst_t<List, bool>, tl::TypeList<int, float>>);
    static_assert(tl::Same_v<tl::RemoveFirst_t<List, double>, tl::TypeList<int, float, bool>>);

    static_assert(tl::Same_v<tl::RemoveFirst_t<tl::EmptyTypeList, double>, tl::EmptyTypeList>);
    static_assert(tl::Same_v<
            tl::RemoveFirst_t<
                    tl::RemoveFirst_t<
                            tl::RemoveFirst_t<tl::TypeList<bool, bool, bool>, bool>,
                            bool>,
                    bool>,
            tl::EmptyTypeList
    >);
    static_assert(tl::Same_v<
            tl::RemoveFirst_t<tl::TypeList<bool, int, bool, double, bool>, bool>,
            tl::TypeList<int, bool, double, bool>
    >);


    // RemoveAll
    static_assert(tl::Same_v<tl::RemoveAll_t<List, int>, tl::TypeList<float, bool>>);
    static_assert(tl::Same_v<tl::RemoveAll_t<List, float>, tl::TypeList<int, bool>>);
    static_assert(tl::Same_v<tl::RemoveAll_t<List, bool>, tl::TypeList<int, float>>);
    static_assert(tl::Same_v<tl::RemoveAll_t<List, double>, tl::TypeList<int, float, bool>>);

    static_assert(tl::Same_v<tl::RemoveAll_t<tl::EmptyTypeList, double>, tl::EmptyTypeList>);
    static_assert(tl::Same_v<
            tl::RemoveAll_t<tl::TypeList<bool, bool, bool>, bool>,
            tl::EmptyTypeList
    >);
    static_assert(tl::Same_v<
            tl::RemoveAll_t<tl::TypeList<bool, int, bool, double, bool>, bool>,
            tl::TypeList<int, double>
    >);


    // EraseDuplicates
    static_assert(tl::Same_v<
            tl::EraseDuplicates_t<tl::TypeList<int>>, tl::TypeList<int>
    >);

    static_assert(tl::Same_v<
            tl::EraseDuplicates_t<tl::TypeList<int, int>>, tl::TypeList<int>
    >);

    static_assert(tl::Same_v<
            tl::EraseDuplicates_t<tl::TypeList<int, int, int>>, tl::TypeList<int>
    >);

    static_assert(tl::Same_v<
            tl::EraseDuplicates_t<tl::TypeList<int, char, int, char, float, float, char, int, double, float, double>>,
            tl::TypeList<int, char, float, double>
    >);

    // todo: add ReplaceFirst tests

    // ReplaceAll
    static_assert(tl::Same_v<
            tl::ReplaceAll_t<tl::TypeList<int>, int, float>, tl::TypeList<float>
    >);

    static_assert(tl::Same_v<
            tl::ReplaceAll_t<tl::TypeList<int, int>, int, float>, tl::TypeList<float, float>
    >);

    static_assert(tl::Same_v<
            tl::ReplaceAll_t<tl::TypeList<int, int, int>, int, float>, tl::TypeList<float, float, float>
    >);

    static_assert(tl::Same_v<
            tl::ReplaceAll_t<tl::TypeList<int>, float, int>, tl::TypeList<int>
    >);

    static_assert(tl::Same_v<
            tl::ReplaceAll_t<tl::TypeList<int, double, int>, int, float>, tl::TypeList<float, double, float>
    >);

    static_assert(tl::Same_v<
            tl::ReplaceAll_t<tl::TypeList<double, double, int>, int, float>, tl::TypeList<double, double, float>
    >);

    static_assert(tl::Same_v<
            tl::ReplaceAll_t<tl::TypeList<int, double, double>, int, float>, tl::TypeList<float, double, double>
    >);

    static_assert(tl::Same_v<
            tl::ReplaceAll_t<tl::TypeList<int>, int, int>, tl::TypeList<int>
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
    static_assert(traits::Convertible_v<int, int>);
    static_assert(traits::Convertible_v<int, float>);
    static_assert(traits::Convertible_v<float, int>);

    static_assert(!traits::Convertible_v<int, type_list::EmptyTypeList>);
    static_assert(!traits::Convertible_v<type_list::EmptyTypeList, int>);

    static_assert(traits::Convertible_v<A1, A>);
    static_assert(!traits::Convertible_v<A, A1>);
    static_assert(traits::Convertible_v<A2, A>);
    static_assert(!traits::Convertible_v<A2, A1>);

    static_assert(traits::Convertible_v<A11, A>);
    static_assert(traits::Convertible_v<A11, A1>);
    static_assert(!traits::Convertible_v<A11, A2>);
}


void testHierarchy() {
    using traits::SameTypes_v;

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
    static_assert(SameTypes_v<h::MostDerived_t<tl::TypeList<A>, A>, A>);

    static_assert(SameTypes_v<h::MostDerived_t<tl::TypeList<A, A1>, A>, A1>);
    static_assert(SameTypes_v<h::MostDerived_t<tl::TypeList<A1, A>, A>, A1>);

    static_assert(SameTypes_v<h::MostDerived_t<tl::TypeList<A, A1, A11>, A>, A11>);
    static_assert(SameTypes_v<h::MostDerived_t<tl::TypeList<A1, A11, A>, A>, A11>);
    static_assert(SameTypes_v<h::MostDerived_t<tl::TypeList<A11, A, A1>, A>, A11>);
    static_assert(SameTypes_v<h::MostDerived_t<tl::TypeList<A, A11, A1>, A>, A11>);
    static_assert(SameTypes_v<h::MostDerived_t<tl::TypeList<A1, A, A11>, A>, A11>);
    static_assert(SameTypes_v<h::MostDerived_t<tl::TypeList<A11, A1, A>, A>, A11>);


    // DerivedToFront
    static_assert(tl::Same_v<
            h::DerivedToFront_t<tl::TypeList<B>>,
            tl::TypeList<B>
    >);

    static_assert(tl::Same_v<
            h::DerivedToFront_t<tl::TypeList<B, B1>>,
            tl::TypeList<B1, B>
    >);

    static_assert(tl::Same_v<
            h::DerivedToFront_t<tl::TypeList<B1, B>>,
            tl::TypeList<B1, B>
    >);

    static_assert(tl::Same_v<
            h::DerivedToFront_t<tl::TypeList<B, A, A1, B1>>,
            tl::TypeList<B1, A1, A, B>
    >);

    static_assert(tl::Same_v<
            h::DerivedToFront_t<tl::TypeList<B, A1, B2, A, A2, B21, B1>>,
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
