//
// Created by Admin on 11/03/2025.
//

#pragma once

#include "FieldInfo.h"

namespace My::MyDRefl {
struct TypeInfo {
  std::unordered_map<size_t, std::any> attrs;
  std::unordered_map<size_t, FieldInfo> fieldinfos;
  std::unordered_map<size_t, StaticFieldInfo> staticfieldinfos;
};
}  // namespace My::MyDRefl
