//
// Created by Admin on 1/04/2025.
//

#pragma once

#include "Method.h"
#include "Attr.h"

namespace My::MyDRefl {
struct MethodInfo {
  Method method;
  std::unordered_map<TypeID, Attr> attrs;
};
}  // namespace My::MyDRefl
