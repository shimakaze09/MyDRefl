//
// Created by Admin on 31/03/2025.
//

#pragma once

#include "FieldPtr.h"

#include <any>
#include <unordered_map>

namespace My::MyDRefl {
struct FieldInfo {
  FieldPtr fieldptr;
  std::unordered_map<size_t, std::any> attrs;
};

struct ConstFieldInfo {
  ConstFieldPtr fieldptr;
  std::unordered_map<size_t, std::any> attrs;
};

struct StaticFieldInfo {
  ObjectPtr objptr;
  std::unordered_map<size_t, std::any> attrs;
};

struct StaticConstFieldInfo {
  ConstObjectPtr objptr;
  std::unordered_map<size_t, std::any> attrs;
};
}  // namespace My::MyDRefl
