#pragma once

#include <memory>
#include "typelist.h"
#include "tuple.h"

namespace functor {

template <int args_left, typename Result, typename Tuple, typename Invoker,
          typename... CollectedArgs>
traits::EnableIfT<args_left != 0, Result> InvokeTupleArgs(Invoker invoker, Tuple tuple,
                                                          CollectedArgs... args) {
    return InvokeTupleArgs<args_left - 1, Result>(invoker, tuple,
                                                  tuple::TupleGet<args_left - 1>(tuple), args...);
}

template <int args_left, typename Result, typename Tuple, typename Invoker,
          typename... CollectedArgs>
traits::EnableIfT<args_left == 0, Result> InvokeTupleArgs(Invoker invoker, Tuple tuple,
                                                          CollectedArgs... args) {
    return invoker->invoke(args...);
}

template <typename Result, typename FunctionType>
struct FreeFunctionHolder {
    template <typename... CallArgs>
    Result operator()(CallArgs... args) {
        return function(args...);
    }

    FreeFunctionHolder(FunctionType function) : function(function) {
    }

    template <typename... CallArgs>
    Result invoke(CallArgs... args) {
        return operator()(args...);
    }

    FunctionType function;
};

template <typename FunctionType, typename... Args>
class Functor {
    using Tuple = tuple::Tuple<Args...>;
    using Result = traits::GetFunctionResultTypeT<FunctionType>;
    using Invoker = std::shared_ptr<FreeFunctionHolder<Result, FunctionType>>;

public:
    Functor(FunctionType func, Args... args)
        : invoker(new FreeFunctionHolder<Result, FunctionType>(func)) {
        tuple::TupleAssign<0>(tuple, args...);
    }

    template <typename... CallArgs>
    Result operator()(CallArgs... args) {
        return InvokeTupleArgs<accepted_args_length, Result>(invoker, tuple, args...);
    }

private:
    Tuple tuple;
    static constexpr int accepted_args_length = typelist::LengthV<typelist::TypeList<Args...>>;
    Invoker invoker;
};

}  // namespace functor