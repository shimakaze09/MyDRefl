//
// Created by Admin on 2/04/2025.
//

#pragma once

#include <MyTemplate/Util.h>

#include <string>
#include <unordered_map>

namespace My::MyDRefl {
class IDRegistry {
 public:
  static constexpr size_t DirectGetID(std::string_view name) noexcept {
    return string_hash(name);
  }

  void Register(size_t ID, std::string_view name);
  size_t GetID(std::string_view name);
  std::string_view Nameof(size_t ID) const noexcept;

 private:
  std::unordered_map<size_t, std::string> id2name;
};
}  // namespace My::MyDRefl

#include "details/IDRegistry.inl"
