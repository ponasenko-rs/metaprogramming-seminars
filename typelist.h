#pragma once

#include <type_traits>

namespace type_list {
    struct NullType {
    };

    template<typename ...Args>
    struct TypeList {
//        using Head = NullType;
//        using Tail = NullType;
    };

    using EmptyTypeList = TypeList<>;

    template<typename H, typename ...T>
    struct TypeList<H, T...> {
        using Head = H;
        using Tail = TypeList<T...>;
    };

    template<typename H, typename ...T>
    struct TypeList<H, TypeList<T...>> {
        using Head = H;
        using Tail = TypeList<T...>;
    };


//    template<typename H>
//    struct TypeList<H, EmptyTypeList> {
//        using Head = H;
//        using Tail = EmptyTypeList;
//    };

    template<typename H>
    struct TypeList<H> {
        using Head = H;
        using Tail = EmptyTypeList;
    };




    // Functions

    // Length
    template<typename List>
    struct Length {
        static const int value = 1 + Length<typename List::Tail>::value;
    };

    template<>
    struct Length<EmptyTypeList> {
        static const int value = 0;
    };

    template<typename List>
    static const int Length_v = Length<List>::value;


    // At
    template<typename List, int index>
    struct At {
        static_assert(index > 0);
        using type = typename At<typename List::Tail, index - 1>::type;
    };

    template<int index>
    struct At<EmptyTypeList, index> {
        using type = NullType;
    };

    template<typename List>
    struct At<List, 0> {
        using type = typename List::Head;
    };

    template<>
    struct At<EmptyTypeList, 0> {
        using type = NullType;
    };

    template<typename List, int index>
    using At_t = typename At<List, index>::type;



    // Same
    template<typename List1, typename List2>
    struct Same {
        static const bool value = []() {
            if (Length<List1>::value != Length<List2>::value) {
                return false;
            }

            if (!std::is_same_v<typename List1::Head, typename List2::Head>) {
                return false;
            }

            return Same<typename List1::Tail, typename List2::Tail>::value;
        }();
    };

    template<>
    struct Same<EmptyTypeList, EmptyTypeList> {
        static const bool value = true;
    };

    template<typename List>
    struct Same<EmptyTypeList, List> {
        static const bool value = false;
    };

    template<typename List>
    struct Same<List, EmptyTypeList> {
        static const bool value = false;
    };

    template<typename List1, typename List2>
    const static bool Same_v = Same<List1, List2>::value;


    // IndexOf
    template<typename List, typename T>
    struct IndexOf {
    private:
        static const int next_value = IndexOf<typename List::Tail, T>::value;

    public:
        static const int value = (next_value == -1) ? -1 : 1 + next_value;
    };

    template<typename T, typename ...O>
    struct IndexOf<TypeList<T, O...>, T> {
        static const int value = 0;
    };

    template<typename T>
    struct IndexOf<EmptyTypeList, T> {
        static const int value = -1;
    };

    template<typename List, typename T>
    static const int IndexOf_v = IndexOf<List, T>::value;


    // Add
    template<typename List, typename T, int index>
    struct Add {
        using type = TypeList<typename List::Head, typename Add<typename List::Tail, T, index - 1>::type>;
    };

    template<typename T, int index>
    struct Add<EmptyTypeList, T, index> {
        using type = TypeList<T>;
    };

    template<typename List, typename T>
    struct Add<List, T, 0> {
        using type = TypeList<T, List>;
    };

    template<typename T>
    struct Add<EmptyTypeList, T, 0> {
        using type = TypeList<T>;
    };

    template<typename List, typename T, int index>
    using Add_t = typename Add<List, T, index>::type;



    // RemoveFirst
    template<typename List, typename T>
    struct RemoveFirst {
        using type = TypeList<typename List::Head, typename RemoveFirst<typename List::Tail, T>::type>;
    };

    template<typename T, typename ...O>
    struct RemoveFirst<TypeList<T, O...>, T> {
        using type = typename TypeList<T, O...>::Tail;
    };

    template<typename T>
    struct RemoveFirst<EmptyTypeList, T> {
        using type = EmptyTypeList;
    };

    template<typename List, typename T>
    using RemoveFirst_t = typename RemoveFirst<List, T>::type;



    // RemoveAll
    template<typename List, typename T>
    struct RemoveAll {
        using type = TypeList<typename List::Head, typename RemoveAll<typename List::Tail, T>::type>;
    };

    template<typename T, typename ...O>
    struct RemoveAll<TypeList<T, O...>, T> {
        using type = typename RemoveAll<typename TypeList<T, O...>::Tail, T>::type;
    };

    template<typename T>
    struct RemoveAll<EmptyTypeList, T> {
        using type = EmptyTypeList;
    };

    template<typename List, typename T>
    using RemoveAll_t = typename RemoveAll<List, T>::type;


    // EraseDuplicates
    template<typename List>
    struct EraseDuplicates {
        using type = TypeList<
                typename List::Head,
                typename RemoveAll<
                        typename EraseDuplicates<typename List::Tail>::type,
                        typename List::Head
                >::type
        >;
    };

    template<>
    struct EraseDuplicates<EmptyTypeList> {
        using type = EmptyTypeList;
    };

    template<typename List>
    using EraseDuplicates_t = typename EraseDuplicates<List>::type;



    // ReplaceFirst
    template<typename List, typename T, typename R>
    struct ReplaceFirst {
        using type = TypeList<typename List::Head, typename ReplaceFirst<typename List::Tail, T, R>::type>;
    };

    template<typename List, typename T>
    struct ReplaceFirst<List, T, T> {
        using type = List;
    };

    template<typename T, typename ...O>
    struct ReplaceFirst<TypeList<T, O...>, T, T> {
        using type = TypeList<T, O...>;
    };

    template<typename T>
    struct ReplaceFirst<EmptyTypeList, T, T> {
        using type = EmptyTypeList;
    };

    template<typename T, typename R, typename ...O>
    struct ReplaceFirst<TypeList<T, O...>, T, R> {
        using type = TypeList<R, O...>;
    };

    template<typename T, typename R>
    struct ReplaceFirst<EmptyTypeList, T, R> {
        using type = EmptyTypeList;
    };

    template<typename List, typename T, typename R>
    using ReplaceFirst_t = typename ReplaceFirst<List, T, R>::type;



    // ReplaceAll
    template<typename List, typename T, typename R>
    struct ReplaceAll {
        using type = TypeList<typename List::Head, typename ReplaceAll<typename List::Tail, T, R>::type>;
    };

    template<typename List, typename T>
    struct ReplaceAll<List, T, T> {
        using type = List;
    };

    template<typename T, typename ...O>
    struct ReplaceAll<TypeList<T, O...>, T, T> {
        using type = TypeList<T, O...>;
    };

    template<typename T>
    struct ReplaceAll<EmptyTypeList, T, T> {
        using type = EmptyTypeList;
    };

    template<typename T, typename R, typename ...O>
    struct ReplaceAll<TypeList<T, O...>, T, R> {
        using type = TypeList<R, typename ReplaceAll<typename TypeList<T, O...>::Tail, T, R>::type>;
    };

    template<typename T, typename R>
    struct ReplaceAll<EmptyTypeList, T, R> {
        using type = EmptyTypeList;
    };

    template<typename List, typename T, typename R>
    using ReplaceAll_t = typename ReplaceAll<List, T, R>::type;

} // namespace type_list

