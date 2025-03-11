//
// Created by Admin on 11/03/2025.
//

#pragma once

#include "AnyWrapper.h"
#include "FuncSig.h"

namespace My::MyDRefl {
// TODO : compact
class ArgList {
 public:
  template <typename... Args>
  ArgList(Args... args) {
    (Append<Args>(std::forward<Args>(args)), ...);
  }

  template <typename Arg>
  void Append(Arg arg) {
    args.emplace_back(std::forward<Arg>(arg));
    signature.argHashcodes.push_back(typeid(Arg).hash_code());
  }

  template <typename T>
  T&& GetArg(size_t i) {
    return std::forward<T>(args[i].Cast<std::decay_t<T>>());
  }

  const FuncSig& GetFuncSig() { return signature; }

 private:
  FuncSig signature;
  std::vector<AnyWrapper> args;
};
}  // namespace My::MyDRefl
