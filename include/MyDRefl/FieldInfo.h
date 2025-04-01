//
// Created by Admin on 31/03/2025.
//

#pragma once

#include "FieldPtr.h"
#include "Attr.h"

#include <unordered_map>

namespace My::MyDRefl {
struct FieldInfo {
  FieldPtr fieldptr;
  std::unordered_map<TypeID, Attr> attrs;
};
}  // namespace My::MyDRefl
