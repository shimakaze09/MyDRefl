//
// Created by Admin on 1/04/2025.
//

#pragma once

#include "EnumInfo.h"
#include "NameRegistry.h"
#include "TypeInfo.h"

namespace My::MyDRefl {
class ReflMngr {
 public:
  static ReflMngr& Instance() noexcept {
    static ReflMngr instance;
    return instance;
  }

  NameRegistry registry;
  std::unordered_map<size_t, TypeInfo> typeinfos;
  std::unordered_map<size_t, EnumInfo> enuminfos;

 private:
  ReflMngr() = default;
};
}  // namespace My::MyDRefl
