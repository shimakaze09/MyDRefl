//
// Created by Admin on 2/04/2025.
//

#pragma once

#include <type_traits>

namespace My::MyDRefl {
class NameID {
 public:
  constexpr NameID() noexcept : value{static_cast<size_t>(-1)} {}

  constexpr NameID(size_t value) noexcept : value{value} {}

  constexpr size_t GetValue() const noexcept { return value; }

  constexpr bool IsValid() const noexcept { return value != NameID{}.value; }

  explicit constexpr operator size_t() const noexcept { return value; }

  explicit constexpr operator bool() const noexcept { return IsValid(); }

  constexpr bool operator<(const NameID& rhs) const noexcept {
    return value < rhs.value;
  }

  constexpr bool operator<=(const NameID& rhs) const noexcept {
    return value <= rhs.value;
  }

  constexpr bool operator>(const NameID& rhs) const noexcept {
    return value > rhs.value;
  }

  constexpr bool operator>=(const NameID& rhs) const noexcept {
    return value >= rhs.value;
  }

  constexpr bool operator==(const NameID& rhs) const noexcept {
    return value == rhs.value;
  }

  constexpr bool operator!=(const NameID& rhs) const noexcept {
    return value != rhs.value;
  }

 private:
  size_t value;
};

class TypeID {
 public:
  constexpr TypeID() noexcept : value{static_cast<size_t>(-1)} {}

  constexpr TypeID(size_t value) noexcept : value{value} {}

  constexpr size_t GetValue() const noexcept { return value; }

  constexpr bool IsValid() const noexcept { return value != TypeID{}.value; }

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

namespace std {
template <typename T>
struct hash;

template <>
struct hash<My::MyDRefl::NameID> {
  size_t operator()(const My::MyDRefl::NameID& ID) const noexcept {
    return std::hash<size_t>{}(ID.GetValue());
  }
};

template <>
struct hash<My::MyDRefl::TypeID> {
  size_t operator()(const My::MyDRefl::TypeID& ID) const noexcept {
    return std::hash<size_t>{}(ID.GetValue());
  }
};
}  // namespace std
