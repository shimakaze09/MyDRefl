//
// Created by Admin on 1/04/2025.
//

#pragma once

#include "Method.h"

namespace My::MyDRefl {
struct MethodInfo {
  Method method;
  std::unordered_map<NameID, std::any> attrs;
};
}  // namespace My::MyDRefl
