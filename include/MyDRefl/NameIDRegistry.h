//
// Created by Admin on 2/04/2025.
//

#pragma once

#include "NameID.h"

namespace My::MyDRefl {
class NameIDRegistry {
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

  NameIDRegistry();

  //
  // API
  ////////

  NameID GetID(std::string_view name) { return NameID{registry.GetID(name)}; }

  std::string_view Nameof(NameID ID) const noexcept {
    return registry.Nameof(ID.GetValue());
  }

 private:
  IDRegistry registry;
};
}  // namespace My::MyDRefl
