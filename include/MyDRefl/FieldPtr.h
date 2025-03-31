//
// Created by Admin on 31/03/2025.
//

#pragma once

#include "Object.h"

#include <cassert>

namespace My::MyDRefl {
class FieldPtr {
  struct Void {};

 public:
  template <typename Obj, typename Value>
  FieldPtr(size_t objID, size_t valueID, Value Obj::* ptr)
      : objID{objID},
        valueID{valueID},
        ptr{reinterpret_cast<Void Void::*>(ptr)} {}

  ObjectPtr Map(ObjectPtr objptr) const noexcept {
    assert(objptr.GetID() == objID);
    return {valueID, &(objptr.As<Void>().*ptr)};
  }

  ConstObjectPtr Map(ConstObjectPtr objptr) const noexcept {
    return Map(reinterpret_cast<ObjectPtr&>(objptr));
  }

 private:
  size_t objID;
  size_t valueID;
  Void Void::* ptr;
};
}  // namespace My::MyDRefl
