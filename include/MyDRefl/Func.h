//
// Created by Admin on 11/03/2025.
//

#pragma once

#include "ArgList.h"

#include <cassert>
#include <functional>

namespace My::MyDRefl {
struct MYDREFL_DESC Func {
  std::function<AnyWrapper(ArgList)> func;
  FuncSig signature;

  template <typename T>
  Func(T&& func);

  template <typename... Args>
  bool SignatureIs() const noexcept {
    return signature.Is<Args...>();
  }

  AnyWrapper Call(ArgList arglist) {
    assert(signature == arglist.GetFuncSig());
    return func(std::move(arglist));
  }

  template <typename Ret, typename... Args>
  Ret Call(Args... args);
};
}  // namespace My::MyDRefl

#include "detail/Func.inl"
