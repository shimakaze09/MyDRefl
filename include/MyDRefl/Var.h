//
// Created by Admin on 11/03/2025.
//

#pragma once

#include "Object.h"

#include <any>
#include <cassert>
#include <functional>

namespace My::MyDRefl {
struct Var {
  std::any getter;

  template <typename T>
  static Var Init(size_t offset) {
    return {std::function{[=](Object obj) -> T& {
      return obj.Var<T>(offset);
    }}};
  }

  template <typename T>
  bool TypeIs() const noexcept {
    return getter.type() == typeid(std::function<T&(Object)>);
  }

  template <typename T>
  T& Get(Object obj) const {
    assert(TypeIs<T>());
    return std::any_cast<std::function<T&(Object)>>(getter)(obj);
  }

  template <typename Arg>
  void Set(Object obj, Arg arg) const {
    Get<Arg>(obj) = std::forward<Arg>(arg);
  }
};
}  // namespace My::MyDRefl
