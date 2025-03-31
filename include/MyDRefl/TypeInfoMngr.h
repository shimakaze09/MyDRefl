//
// Created by Admin on 11/03/2025.
//

#pragma once

#include "TypeInfo.h"

namespace My::MyDRefl {
class TypeInfoMngr {
 public:
  static TypeInfoMngr& Instance() noexcept {
    static TypeInfoMngr instance;
    return instance;
  }

  std::unordered_map<size_t, TypeInfo> typeinfos;

 private:
  TypeInfoMngr() = default;
};
}  // namespace My::MyDRefl
