//
// Created by Admin on 31/03/2025.
//

#pragma once

#include "AttrSet.h"
#include "FieldPtr.h"

namespace My::MyDRefl {
struct FieldInfo {
  FieldPtr fieldptr;
  AttrSet attrs;
};

struct Field {
  StrID ID;
  const FieldInfo& info;
};
}  // namespace My::MyDRefl
