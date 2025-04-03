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
  void Clear();

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

    static constexpr char void_n[] = "void";
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
    RegisterUnmanaged(type_name<T>());
  }

  template <typename T>
  bool IsRegistered() const noexcept {
    return IDRegistry<TypeID>::IsRegistered(type_name<T>());
  }
};
}  // namespace My::MyDRefl

#include "details/IDRegistry.inl"
