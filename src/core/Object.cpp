//
// Created by Admin on 11/03/2025.
//

#include <MyDRefl/Object.h>

#include <MyDRefl/TypeInfoMngr.h>

using namespace My::MyDRefl;

TypeInfo* Object::GetTypeInfo() const {
  assert(id != static_cast<size_t>(-1));
  return TypeInfoMngr::Instance().GetTypeInfo(id);
}
