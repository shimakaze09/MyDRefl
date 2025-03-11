//
// Created by Admin on 11/03/2025.
//

#pragma once

#include <unordered_map>
#include "Export.h"
#include "TypeInfo.h"

namespace My::MyDRefl {
class MYDREFL_DESC TypeInfoMngr {
 public:
  static TypeInfoMngr& Instance() {
    static TypeInfoMngr instance;
    return instance;
  }

  TypeInfo* GetTypeInfo(size_t id);

 private:
  std::unordered_map<size_t, TypeInfo*> id2typeinfo;

  TypeInfoMngr() = default;
  ~TypeInfoMngr();
};
}  // namespace My::MyDRefl
