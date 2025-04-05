//
// Created by Admin on 2/04/2025.
//

#pragma once

#include "Object.h"

#include <set>

namespace My::MyDRefl {
using Attr = SharedObject;

struct AttrLess {
  using is_transparent = int;

  bool operator()(const Attr& lhs, const Attr& rhs) const noexcept {
    return lhs.GetID() < rhs.GetID();
  }

  bool operator()(const Attr& lhs, const TypeID& rhs) const noexcept {
    return lhs.GetID() < rhs;
  }

  bool operator()(const TypeID& lhs, const Attr& rhs) const noexcept {
    return lhs < rhs.GetID();
  }
};

using AttrSet = std::set<Attr, AttrLess>;
}  // namespace My::MyDRefl
