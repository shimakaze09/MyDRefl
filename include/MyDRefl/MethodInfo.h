//
// Created by Admin on 1/04/2025.
//

#pragma once

#include "AttrSet.h"
#include "MethodPtr.h"

namespace My::MyDRefl {
struct MethodInfo {
  MethodPtr methodptr;
  AttrSet attrs;
};
}  // namespace My::MyDRefl
