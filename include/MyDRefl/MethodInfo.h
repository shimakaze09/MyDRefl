//
// Created by Admin on 1/04/2025.
//

#pragma once

#include "Attr.h"
#include "MethodPtr.h"

namespace My::MyDRefl {
struct MethodInfo {
  MethodPtr methodptr;
  std::unordered_map<TypeID, Attr> attrs;
};

struct Method {
  NameID ID;
  MethodInfo info;
};
}  // namespace My::MyDRefl
