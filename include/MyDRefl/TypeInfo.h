//
// Created by Admin on 11/03/2025.
//

#pragma once

#include "BaseInfo.h"
#include "FieldInfo.h"
#include "MethodInfo.h"

namespace My::MyDRefl {
struct TypeInfo {
  //
  // Data
  /////////

  size_t size;
  size_t alignment;
  std::unordered_map<StrID, FieldInfo> fieldinfos;
  std::unordered_multimap<StrID, MethodInfo> methodinfos;
  std::unordered_map<TypeID, BaseInfo> baseinfos;
  AttrSet attrs;
};
}  // namespace My::MyDRefl
