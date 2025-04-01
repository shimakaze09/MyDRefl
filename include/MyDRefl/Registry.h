//
// Created by Admin on 2/04/2025.
//

#pragma once

#include "ID.h"

#include <deque>
#include <string>
#include <unordered_map>

namespace My::MyDRefl {
class Registry {
 public:
  size_t Register(std::string_view name);
  size_t GetID(std::string_view name) const noexcept;

  bool IsRegistered(size_t ID) const noexcept { return ID < names.size(); }

  std::string_view Nameof(size_t ID) const noexcept;

 private:
  std::deque<std::string> names;
  std::unordered_map<std::string_view, size_t> name2id;
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
    // Common
    ///////////

    static constexpr char size[] = "size";
    static constexpr char alignment[] = "alignment";
    static constexpr char ptr[] = "ptr";

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

  NameID Register(std::string_view name) {
    return NameID{registry.Register(name)};
  }

  NameID GetID(std::string_view name) const noexcept {
    return NameID{registry.GetID(name)};
  }

  bool IsRegistered(NameID ID) const noexcept {
    return registry.IsRegistered(ID.GetValue());
  }

  std::string_view Nameof(NameID ID) const noexcept {
    return registry.Nameof(ID.GetValue());
  }

 private:
  Registry registry;
};

class TypeRegistry {
 public:
  struct Meta {
    static constexpr char global[] = "MyDRefl::global";

    //
    // Float
    //////////

    static constexpr char t_float[] = "float";
    static constexpr char t_double[] = "double";

    //
    // Fix Size
    /////////////

    static constexpr char t_int8_t[] = "int8_t";
    static constexpr char t_int16_t[] = "int16_t";
    static constexpr char t_int32_t[] = "int32_t";
    static constexpr char t_int64_t[] = "int64_t";

    static constexpr char t_uint8_t[] = "uint8_t";
    static constexpr char t_uint16_t[] = "uint16_t";
    static constexpr char t_uint32_t[] = "uint32_t";
    static constexpr char t_uint64_t[] = "uint64_t";

    //
    // Common
    ///////////

    static constexpr char t_void_ptr[] = "void*";
    static constexpr char t_const_void_ptr[] = "const void*";
  };

  TypeRegistry();

  TypeID Register(std::string_view name) {
    return TypeID{registry.Register(name)};
  }

  TypeID GetID(std::string_view name) const noexcept {
    return TypeID{registry.GetID(name)};
  }

  bool IsRegistered(TypeID ID) const noexcept {
    return registry.IsRegistered(ID.GetValue());
  }

  std::string_view Nameof(TypeID ID) const noexcept {
    return registry.Nameof(ID.GetValue());
  }

 private:
  Registry registry;
};
}  // namespace My::MyDRefl
