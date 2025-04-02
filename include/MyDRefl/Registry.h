//
// Created by Admin on 2/04/2025.
//

#pragma once

#include "ID.h"

#include <MyTemplate/Name.h>

#include <deque>
#include <string>
#include <unordered_map>

namespace My::MyDRefl {
class Registry {
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

class NameRegistry {
 public:
  struct Meta {
    //
    // Global
    ///////////

    static constexpr char malloc[] = "MyDRefl::malloc";
    static constexpr char free[] = "MyDRefl::free";
    static constexpr char aligned_malloc[] = "MyDRefl::aligned_malloc";
    static constexpr char aligned_free[] = "MyDRefl::aligned_free";

    //
    // Member
    ///////////

    static constexpr char ctor[] = "MyDRefl::ctor";
    static constexpr char dtor[] = "MyDRefl::dtor";

    static constexpr char operator_add[] = "MyDRefl::operator+";
    static constexpr char operator_minus[] = "MyDRefl::operator-";
    static constexpr char operator_mul[] = "MyDRefl::operator*";
    static constexpr char operator_div[] = "MyDRefl::operator/";
    static constexpr char operator_mod[] = "MyDRefl::operator%";

    static constexpr char operator_eq[] = "MyDRefl::operator=";
    static constexpr char operator_ne[] = "MyDRefl::operator!=";
    static constexpr char operator_lt[] = "MyDRefl::operator<";
    static constexpr char operator_le[] = "MyDRefl::operator<=";
    static constexpr char operator_gt[] = "MyDRefl::operator>";
    static constexpr char operator_ge[] = "MyDRefl::operator>=";

    static constexpr char operator_and[] = "MyDRefl::operator&&";
    static constexpr char operator_or[] = "MyDRefl::operator||";
    static constexpr char operator_not[] = "MyDRefl::operator!";

    static constexpr char operator_pos[] = "MyDRefl::operator+";
    static constexpr char operator_neg[] = "MyDRefl::operator-";
    static constexpr char operator_ref[] = "MyDRefl::operator&";
    static constexpr char operator_deref[] = "MyDRefl::operator*";

    static constexpr char operator_inc[] = "MyDRefl::operator++";
    static constexpr char operator_dec[] = "MyDRefl::operator--";

    static constexpr char operator_band[] = "MyDRefl::operator&";
    static constexpr char operator_bor[] = "MyDRefl::operator|";
    static constexpr char operator_bnot[] = "MyDRefl::operator~";
    static constexpr char operator_bxor[] = "MyDRefl::operator^";
    static constexpr char operator_lshift[] = "MyDRefl::operator<<";
    static constexpr char operator_rshift[] = "MyDRefl::operator>>";

    static constexpr char operator_assign[] = "MyDRefl::operator=";
    static constexpr char operator_assign_add[] = "MyDRefl::operator+=";
    static constexpr char operator_assign_minus[] = "MyDRefl::operator-=";
    static constexpr char operator_assign_mul[] = "MyDRefl::operator*=";
    static constexpr char operator_assign_div[] = "MyDRefl::operator/=";
    static constexpr char operator_assign_mod[] = "MyDRefl::operator%=";
    static constexpr char operator_assign_band[] = "MyDRefl::operator&=";
    static constexpr char operator_assign_bor[] = "MyDRefl::operator|=";
    static constexpr char operator_assign_bxor[] = "MyDRefl::operator^=";
    static constexpr char operator_assign_lshift[] = "MyDRefl::operator<<";
    static constexpr char operator_assign_rshift[] = "MyDRefl::operator>>";

    static constexpr char operator_new[] = "MyDRefl::operator new";
    static constexpr char operator_new_array[] = "MyDRefl::operator new[]";
    static constexpr char operator_delete[] = "MyDRefl::operator delete";
    static constexpr char operator_delete_array[] =
        "MyDRefl::operator delete array";

    static constexpr char operator_member[] = "MyDRefl::operator->";
    static constexpr char operator_call[] = "MyDRefl::operator()";
    static constexpr char operator_comma[] = "MyDRefl::operator,";
    static constexpr char operator_subscript[] = "MyDRefl::operator[]";
  };

  NameRegistry();

  //
  // API
  ////////

  static constexpr NameID DirectGetID(std::string_view name) noexcept {
    return NameID{Registry::DirectGetID(name)};
  }

  NameID GetID(std::string_view name) const {
    return NameID{registry.GetID(name)};
  }

  std::string_view Nameof(NameID ID) const noexcept {
    return registry.Nameof(ID.GetValue());
  }

 private:
  mutable Registry registry;
};

class TypeRegistry {
 public:
  struct Meta {
    static constexpr char global[] = "MyDRefl::global";
  };

  TypeRegistry();

  static constexpr TypeID DirectGetID(std::string_view name) noexcept {
    return TypeID{Registry::DirectGetID(name)};
  }

  TypeID GetID(std::string_view name) const {
    return TypeID{registry.GetID(name)};
  }

  std::string_view Nameof(TypeID ID) const noexcept {
    return registry.Nameof(ID.GetValue());
  }

  template <typename T>
  static constexpr TypeID DirectGetID() noexcept {
    return TypeID{Registry::DirectGetID(type_name<T>().name)};
  }

  template <typename T>
  TypeID GetID() const {
    constexpr auto name = type_name<T>().name;
    constexpr size_t ID = string_hash(name);
    registry.Register(ID, name);
    return TypeID{ID};
  }

 private:
  mutable Registry registry;
};
}  // namespace My::MyDRefl

#include "details/Registry.inl"
