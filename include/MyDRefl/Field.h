//
// Created by Admin on 11/03/2025.
//

#pragma once

#include "AttrList.h"
#include "Func.h"
#include "StaticVar.h"
#include "Var.h"
#include "VariantWrapper.h"

namespace My::MyDRefl {
struct MYDREFL_DESC Field {
  VariantWrapper<Var, StaticVar, Func> value;
  AttrList attrs;

  bool operator<(const Field& rhs) const {
    if (!value.TypeIs<Func>() || !rhs.value.TypeIs<Func>())
      return false;
    return value.Cast<Func>().signature < rhs.value.Cast<Func>().signature;
  }
};
}  // namespace My::MyDRefl
