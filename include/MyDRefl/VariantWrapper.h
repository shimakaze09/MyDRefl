//
// Created by Admin on 11/03/2025.
//

#pragma once

#include <variant>

namespace My::MyDRefl {
template <typename... Ts>
struct VariantWrapper {
  std::variant<Ts...> data;

  bool HasValue() const noexcept { return data.valueless_by_exception(); }

  template <typename T>
  bool TypeIs() const noexcept {
    return std::holds_alternative<T>(data);
  }

  template <typename T>
  T& Cast() {
    return std::get<T>(data);
  }

  template <typename T>
  const T& Cast() const {
    return const_cast<VariantWrapper*>(this)->Cast<T>();
  }

  template <typename T>
  T* CastIf() noexcept {
    return std::get_if<T>(&data);
  }

  template <typename T>
  const T* CastIf() const noexcept {
    return const_cast<VariantWrapper*>(this)->CastIf<T>();
  }
};
}  // namespace My::MyDRefl
