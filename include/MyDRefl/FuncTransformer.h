//
// Created by Admin on 11/03/2025.
//

#pragma once

#include "FuncSig.h"

namespace My::MyDRefl {
// [Usage]
// FuncTransformer<decltype(func)>::GetFunc(std::forward<decltype(func)>(func))
// FuncTransformer<decltype(func)>::GetFuncSig()
template <typename T>
struct FuncTransformer;

// kernel case
template <typename Ret, typename... Args>
struct FuncTransformer<Ret(Args...)> {
  template <typename Func>
  static auto GetFunc(Func&& func);

  static FuncSig GetFuncSig() { return {typeid(Args).hash_code()...}; }
};
}  // namespace My::MyDRefl

#include "detail/FuncTransformer.inl"
