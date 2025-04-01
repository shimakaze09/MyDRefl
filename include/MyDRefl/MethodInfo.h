//
// Created by Admin on 1/04/2025.
//

#pragma once

#include "Method.h"

namespace My::MyDRefl {
struct MethodInfo {
  Method method;
  std::unordered_map<size_t, std::any> attrs;
};

struct ConstMethodInfo {
  ConstMethod method;
  std::unordered_map<size_t, std::any> attrs;
};

struct StaticMethodInfo {
  StaticMethod method;
  std::unordered_map<size_t, std::any> attrs;
};
}  // namespace My::MyDRefl
