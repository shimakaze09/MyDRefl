//
// Created by Admin on 2/04/2025.
//

#pragma once

#include "IDRegistry.h"

#include <MyTemplate/Name.h>

namespace My::MyDRefl {
class TypeID {
 public:
  static constexpr size_t InvalidValue() noexcept {
    return static_cast<size_t>(-1);
  }

  constexpr TypeID() noexcept : value{InvalidValue()} {}

  constexpr TypeID(size_t value) noexcept : value{value} {}

  constexpr TypeID(std::string_view name) noexcept
      : value{IDRegistry::DirectGetID(name)} {}

  template <typename T>
  static constexpr TypeID Of() noexcept {
    return type_name<T>().name;
  }

  constexpr size_t GetValue() const noexcept { return value; }

  constexpr bool IsValid() const noexcept { return value != InvalidValue(); }

  constexpr void Reset() noexcept { value = InvalidValue(); }

  explicit constexpr operator size_t() const noexcept { return value; }

  explicit constexpr operator bool() const noexcept { return IsValid(); }

  constexpr bool operator<(const TypeID& rhs) const noexcept {
    return value < rhs.value;
  }

  constexpr bool operator<=(const TypeID& rhs) const noexcept {
    return value <= rhs.value;
  }

  constexpr bool operator>(const TypeID& rhs) const noexcept {
    return value > rhs.value;
  }

  constexpr bool operator>=(const TypeID& rhs) const noexcept {
    return value >= rhs.value;
  }

  constexpr bool operator==(const TypeID& rhs) const noexcept {
    return value == rhs.value;
  }

  constexpr bool operator!=(const TypeID& rhs) const noexcept {
    return value != rhs.value;
  }

 private:
  size_t value;
};
}  // namespace My::MyDRefl

template <>
struct std::hash<My::MyDRefl::TypeID> {
  size_t operator()(const My::MyDRefl::TypeID& ID) const noexcept {
    return std::hash<size_t>{}(ID.GetValue());
  }
};
