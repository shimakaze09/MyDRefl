//
// Created by Admin on 11/03/2025.
//

#include <MyDRefl/TypeInfoMngr.h>

using namespace My::MyDRefl;

TypeInfo* TypeInfoMngr::GetTypeInfo(size_t id) {
  auto target = id2typeinfo.find(id);
  if (target != id2typeinfo.end())
    return target->second;

  auto [iter, success] = id2typeinfo.try_emplace(id, new TypeInfo(id));
  assert(success);
  return iter->second;
}

TypeInfoMngr::~TypeInfoMngr() {
  for (const auto& [id, typeinfo] : id2typeinfo)
    delete (typeinfo);
}
