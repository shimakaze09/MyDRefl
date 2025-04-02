//
// Created by Admin on 2/04/2025.
//

#pragma once

#include <cassert>

namespace My::MyDRefl {
inline void IDRegistry::Register(size_t ID, std::string_view name) {
  auto target = id2name.find(ID);
  if (target == id2name.end())
    id2name.emplace_hint(target, std::pair{ID, std::string{name}});
  else
    assert(target->second == name);
}

inline std::string_view IDRegistry::Nameof(size_t ID) const noexcept {
  auto target = id2name.find(ID);
  if (target != id2name.end())
    return target->second;
  else
    return {};
}
}  // namespace My::MyDRefl
