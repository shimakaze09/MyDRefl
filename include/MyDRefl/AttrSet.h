//
// Created by Admin on 2/04/2025.
//

#pragma once

#include "SharedObject.h"

#include <unordered_set>

namespace My::MyDRefl {
using Attr = SharedObject;

struct AttrEqual {
  using is_transparent = int;

  bool operator()(const Attr& lhs, const Attr& rhs) const noexcept {
    return lhs.GetID() == rhs.GetID();
  }

  bool operator()(const Attr& lhs, const TypeID& rhs) const noexcept {
    return lhs.GetID() == rhs;
  }

  bool operator()(const TypeID& lhs, const Attr& rhs) const noexcept {
    return lhs == rhs.GetID();
  }
};

struct AttrHash {
  size_t operator()(const Attr& attr) const noexcept {
    return attr.GetID().GetValue();
  }
};

using AttrSet = std::unordered_set<Attr, AttrHash, AttrEqual>;
}  // namespace My::MyDRefl
