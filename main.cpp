#include <iostream>
#include <type_traits>

#include "typelist.h"

void testTypeList() {
    using std::is_same_v;
    namespace tl = type_list;

    using List = tl::TypeList<int, float, bool>;


    // Iterating
    static_assert(is_same_v<List::Head, int>);
    static_assert(is_same_v<List::Tail::Head, float>);
    static_assert(is_same_v<List::Tail::Tail::Head, bool>);
    static_assert(is_same_v<List::Tail::Tail::Tail, tl::EmptyTypeList>);

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
    static_assert(is_same_v<tl::At_t<List, 0>, int>);
    static_assert(is_same_v<tl::At_t<List, 1>, float>);
    static_assert(is_same_v<tl::At_t<List, 2>, bool>);
    static_assert(is_same_v<tl::At_t<List, 3>, tl::NullType>);
    static_assert(is_same_v<tl::At_t<List, 4>, tl::NullType>);


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

int main() {
    testTypeList();

    return 0;
}
