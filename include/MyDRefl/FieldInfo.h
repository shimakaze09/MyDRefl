//
// Created by Admin on 31/03/2025.
//

#pragma once

#include "FieldPtr.h"
#include "NameID.h"
#include "SharedBlock.h"

#include <unordered_map>

namespace My::MyDRefl {
struct FieldInfo {
  FieldPtr fieldptr;
  std::unordered_map<TypeID, SharedBlock> attrs;
};

struct Field {
  NameID ID;
  const FieldInfo& info;
};
}  // namespace My::MyDRefl
