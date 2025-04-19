#pragma once

#include <MySmallFlat/small_vector.hpp>

#include "../Object.hpp"

namespace Smkz::MyDRefl::Ranges {
struct MyDRefl_core_API Derived {
  ObjectView obj;
  TypeInfo* typeinfo;  // not nullptr
  std::unordered_map<Type, BaseInfo>::iterator curbase;

  MyDRefl_core_API friend bool operator==(const Derived& lhs,
                                          const Derived& rhs) {
    return lhs.obj.GetType() == rhs.obj.GetType() &&
           lhs.obj.GetPtr() == rhs.obj.GetPtr() && lhs.curbase == rhs.curbase;
  }
};
}  // namespace Smkz::MyDRefl::Ranges
