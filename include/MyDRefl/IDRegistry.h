//
// Created by Admin on 2/04/2025.
//

#pragma once

#include <MyTemplate/TypeID.h>

#include <deque>
#include <memory_resource>
#include <string>
#include <unordered_map>

#ifndef NDEBUG
#include <unordered_set>
#endif  // !NDEBUG

namespace My::MyDRefl {
template <typename T>
class IDRegistry {
 protected:
  void RegisterUnmanaged(T ID, std::string_view name);
  void Register(T ID, std::string_view name);

 public:
  IDRegistry();

  bool IsRegistered(T ID) const noexcept;
  std::string_view Nameof(T ID) const noexcept;

  void UnregisterUnmanaged(T ID) noexcept;
  void Clear() noexcept;

 private:
  std::pmr::monotonic_buffer_resource resource;
  std::pmr::unordered_map<T, std::string_view> id2name;

#ifndef NDEBUG
 public:
  bool IsUnmanaged(T ID) const noexcept;
  void ClearUnmanaged() noexcept;

 private:
  std::pmr::unordered_set<T> unmanagedIDs;
#endif  // NDEBUG
};

class StrIDRegistry : public IDRegistry<StrID> {
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
    static constexpr char operator_assign_lshift[] = "MyDRefl::operator<<=";
    static constexpr char operator_assign_rshift[] = "MyDRefl::operator>>=";

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

  struct MetaID {
    //
    // Global
    ///////////

    static constexpr StrID malloc = Meta::malloc;
    static constexpr StrID free = Meta::free;
    static constexpr StrID aligned_malloc = Meta::aligned_malloc;
    static constexpr StrID aligned_free = Meta::aligned_free;

    //
    // Member
    ///////////

    static constexpr StrID ctor = Meta::ctor;
    static constexpr StrID dtor = Meta::dtor;

    static constexpr StrID operator_add = Meta::operator_add;
    static constexpr StrID operator_minus = Meta::operator_minus;
    static constexpr StrID operator_mul = Meta::operator_mul;
    static constexpr StrID operator_div = Meta::operator_div;
    static constexpr StrID operator_mod = Meta::operator_mod;

    static constexpr StrID operator_eq = Meta::operator_eq;
    static constexpr StrID operator_ne = Meta::operator_ne;
    static constexpr StrID operator_lt = Meta::operator_lt;
    static constexpr StrID operator_le = Meta::operator_le;
    static constexpr StrID operator_gt = Meta::operator_gt;
    static constexpr StrID operator_ge = Meta::operator_ge;

    static constexpr StrID operator_and = Meta::operator_and;
    static constexpr StrID operator_or = Meta::operator_or;
    static constexpr StrID operator_not = Meta::operator_not;

    static constexpr StrID operator_pos = Meta::operator_pos;
    static constexpr StrID operator_neg = Meta::operator_neg;
    static constexpr StrID operator_ref = Meta::operator_ref;
    static constexpr StrID operator_deref = Meta::operator_deref;

    static constexpr StrID operator_inc = Meta::operator_inc;
    static constexpr StrID operator_dec = Meta::operator_dec;

    static constexpr StrID operator_band = Meta::operator_band;
    static constexpr StrID operator_bor = Meta::operator_bor;
    static constexpr StrID operator_bnot = Meta::operator_bnot;
    static constexpr StrID operator_bxor = Meta::operator_bxor;
    static constexpr StrID operator_lshift = Meta::operator_lshift;
    static constexpr StrID operator_rshift = Meta::operator_rshift;

    static constexpr StrID operator_assign = Meta::operator_assign;
    static constexpr StrID operator_assign_add = Meta::operator_assign_add;
    static constexpr StrID operator_assign_minus = Meta::operator_assign_minus;
    static constexpr StrID operator_assign_mul = Meta::operator_assign_mul;
    static constexpr StrID operator_assign_div = Meta::operator_assign_div;
    static constexpr StrID operator_assign_mod = Meta::operator_assign_mod;
    static constexpr StrID operator_assign_band = Meta::operator_assign_band;
    static constexpr StrID operator_assign_bor = Meta::operator_assign_bor;
    static constexpr StrID operator_assign_bxor = Meta::operator_assign_bxor;
    static constexpr StrID operator_assign_lshift =
        Meta::operator_assign_lshift;
    static constexpr StrID operator_assign_rshift =
        Meta::operator_assign_rshift;

    static constexpr StrID operator_new = Meta::operator_new;
    static constexpr StrID operator_new_array = Meta::operator_new_array;
    static constexpr StrID operator_delete = Meta::operator_delete;
    static constexpr StrID operator_delete_array = Meta::operator_delete_array;

    static constexpr StrID operator_member = Meta::operator_member;
    static constexpr StrID operator_call = Meta::operator_call;
    static constexpr StrID operator_comma = Meta::operator_comma;
    static constexpr StrID operator_subscript = Meta::operator_subscript;
  };

  StrIDRegistry();

  StrID RegisterUnmanaged(std::string_view name) {
    StrID ID{name};
    IDRegistry<StrID>::RegisterUnmanaged(ID, name);
    return ID;
  }

  StrID Register(std::string_view name) {
    StrID ID{name};
    IDRegistry<StrID>::Register(ID, name);
    return ID;
  }
};

class TypeIDRegistry : public IDRegistry<TypeID> {
 public:
  struct Meta {
    static constexpr char global[] = "MyDRefl::global";
    static constexpr char t_void[] = "void";
  };

  struct MetaID {
    static constexpr TypeID global = Meta::global;
    static constexpr TypeID t_void = Meta::t_void;
  };

  TypeIDRegistry() { RegisterUnmanaged(Meta::global); }

  TypeID RegisterUnmanaged(std::string_view name) {
    TypeID ID{name};
    IDRegistry<TypeID>::RegisterUnmanaged(ID, name);
    return ID;
  }

  TypeID Register(std::string_view name) {
    TypeID ID{name};
    IDRegistry<TypeID>::Register(ID, name);
    return ID;
  }

  // unmanaged
  template <typename T>
  void Register() {
    IDRegistry<TypeID>::RegisterUnmanaged(TypeID::of<T>, type_name<T>());
  }

  template <typename T>
  bool IsRegistered() const noexcept {
    return IDRegistry<TypeID>::IsRegistered(TypeID::of<T>);
  }
};
}  // namespace My::MyDRefl

#include "details/IDRegistry.inl"
