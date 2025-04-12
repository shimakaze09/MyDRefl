#pragma once

#include "AttrSet.h"
#include "FieldPtr.h"

namespace My::MyDRefl {
struct FieldInfo {
  FieldPtr fieldptr;
  AttrSet attrs;
};
}  // namespace My::MyDRefl
