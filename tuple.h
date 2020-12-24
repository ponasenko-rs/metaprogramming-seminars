#pragma once

#include "hierarchy.h"

namespace tuple {

template <typename T, typename Base>
struct TupleUnit : public Base {
    T value;
};

template <typename... Args>
struct Tuple : public hierarchy::GenLinearHierarchy<typelist::TypeList<Args...>, TupleUnit> {};

template <int index, typename Tuple>
auto& TupleGet(Tuple& tuple) {
    return hierarchy::LinearHierarchyGet<index>(tuple).value;
}

template <int start_index, typename Tuple, typename... Args>
void TupleAssign(Tuple& tuple, Args... args) {
}

template <int start_index, typename Tuple, typename Arg, typename... Args>
void TupleAssign(Tuple& tuple, Arg arg, Args... args) {
    TupleGet<start_index>(tuple) = arg;
    TupleAssign<start_index + 1>(tuple, args...);
}

}  // namespace tuple