//
// Created by Admin on 31/03/2025.
//

#pragma once

#include "Attr.h"
#include "FieldPtr.h"

#include <unordered_map>

namespace My::MyDRefl {
struct FieldInfo {
  FieldPtr fieldptr;
  std::unordered_map<TypeID, Attr> attrs;
};

struct Field {
  NameID ID;
  const FieldInfo& info;
};
}  // namespace My::MyDRefl
