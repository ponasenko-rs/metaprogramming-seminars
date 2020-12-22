#pragma once

#include <functional>

#include "typelist.h"
#include "hierarchy.h"

namespace functor {

template <int args_left, typename Result, typename Tuple, typename FuncHolder,
          typename... CollectedArgs>
inline typename std::enable_if<args_left != 0, Result>::type ExtractTuple(FuncHolder func_holder,
                                                                          Tuple tuple,
                                                                          CollectedArgs... args) {
    return ExtractTuple<args_left - 1, Result>(
        func_holder, tuple, hierarchy::LinearHierarchyGet<args_left - 1>(tuple).value, args...);
}

template <int args_left, typename Result, typename Tuple, typename FuncHolder,
          typename... CollectedArgs>
inline typename std::enable_if<args_left == 0, Result>::type ExtractTuple(FuncHolder func_holder,
                                                                          Tuple tuple,
                                                                          CollectedArgs... args) {
    return func_holder(args...);
}

template <typename Result, typename FuncType>
struct FunctionHolder {
    template <typename... CallArgs>
    Result operator()(CallArgs... args) {
        return function(args...);
    }

    FunctionHolder(std::function<FuncType> function) : function(function) {
    }
    std::function<FuncType> function;
};

template <typename Result, typename FuncType, typename... Args>
struct Functor {
    using Func = std::function<FuncType>;
    using Tuple =
        hierarchy::GenLinearHierarchy<typelist::TypeList<Args...>, hierarchy::SimpleLinearUnit>;

    Tuple tuple;
    static constexpr int accepted_args_length = typelist::LengthV<typelist::TypeList<Args...>>;
    FunctionHolder<Result, FuncType> func_holder;

    Functor(Func func, Args... args) : func_holder(func) {
        hierarchy::LinearHierarchyAssign<0>(tuple, args...);
    }

    template <typename... CallArgs>
    Result operator()(CallArgs... args) {
        return ExtractTuple<accepted_args_length, Result>(func_holder, tuple, args...);
    }
};

}  // namespace functor