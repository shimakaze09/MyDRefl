//
// Created by Admin on 1/04/2025.
//

#pragma once

#include "MethodPtr.h"
#include "NameID.h"
#include "SharedBlock.h"

namespace My::MyDRefl {
struct MethodInfo {
  MethodPtr methodptr;
  std::unordered_map<TypeID, SharedBlock> attrs;
};

struct Method {
  NameID ID;
  MethodInfo info;
};
}  // namespace My::MyDRefl
