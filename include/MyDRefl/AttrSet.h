#pragma once

#include "Object.h"

#include <set>

namespace My::MyDRefl {
using Attr = SharedObject;

struct AttrLess {
  using is_transparent = int;

  bool operator()(const Attr& lhs, const Attr& rhs) const noexcept {
    return lhs.GetType() < rhs.GetType();
  }

  bool operator()(const Attr& lhs, const Type& rhs) const noexcept {
    return lhs.GetType() < rhs;
  }

  bool operator()(const Type& lhs, const Attr& rhs) const noexcept {
    return lhs < rhs.GetType();
  }
};

using AttrSet = std::set<Attr, AttrLess>;
}  // namespace My::MyDRefl
