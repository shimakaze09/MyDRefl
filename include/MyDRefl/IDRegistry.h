//
// Created by Admin on 2/04/2025.
//

#pragma once

#include <MyTemplate/TypeID.h>

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

    static constexpr char operator_plus[] = "MyDRefl::operator+";
    static constexpr char operator_minus[] = "MyDRefl::operator-";

    static constexpr char operator_add[] = "MyDRefl::operator+";
    static constexpr char operator_sub[] = "MyDRefl::operator-";
    static constexpr char operator_mul[] = "MyDRefl::operator*";
    static constexpr char operator_div[] = "MyDRefl::operator/";
    static constexpr char operator_mod[] = "MyDRefl::operator%";

    static constexpr char operator_bnot[] = "MyDRefl::operator~";
    static constexpr char operator_band[] = "MyDRefl::operator&";
    static constexpr char operator_bor[] = "MyDRefl::operator|";
    static constexpr char operator_bxor[] = "MyDRefl::operator^";
    static constexpr char operator_lshift[] = "MyDRefl::operator<<";
    static constexpr char operator_rshift[] = "MyDRefl::operator>>";

    static constexpr char operator_pre_inc[] = "MyDRefl::operator++";
    static constexpr char operator_pre_dec[] = "MyDRefl::operator--";
    static constexpr char operator_post_inc[] = "MyDRefl::operator++";
    static constexpr char operator_post_dec[] = "MyDRefl::operator--";

    static constexpr char operator_assign[] = "MyDRefl::operator=";
    static constexpr char operator_assign_add[] = "MyDRefl::operator+=";
    static constexpr char operator_assign_sub[] = "MyDRefl::operator-=";
    static constexpr char operator_assign_mul[] = "MyDRefl::operator*=";
    static constexpr char operator_assign_div[] = "MyDRefl::operator/=";
    static constexpr char operator_assign_mod[] = "MyDRefl::operator%=";
    static constexpr char operator_assign_band[] = "MyDRefl::operator&=";
    static constexpr char operator_assign_bor[] = "MyDRefl::operator|=";
    static constexpr char operator_assign_bxor[] = "MyDRefl::operator^=";
    static constexpr char operator_assign_lshift[] = "MyDRefl::operator<<=";
    static constexpr char operator_assign_rshift[] = "MyDRefl::operator>>=";

    static constexpr char operator_eq[] = "MyDRefl::operator==";
    static constexpr char operator_ne[] = "MyDRefl::operator!=";
    static constexpr char operator_lt[] = "MyDRefl::operator<";
    static constexpr char operator_le[] = "MyDRefl::operator<=";
    static constexpr char operator_gt[] = "MyDRefl::operator>";
    static constexpr char operator_ge[] = "MyDRefl::operator>=";

    static constexpr char operator_and[] = "MyDRefl::operator&&";
    static constexpr char operator_or[] = "MyDRefl::operator||";
    static constexpr char operator_not[] = "MyDRefl::operator!";

    static constexpr char operator_subscript[] = "MyDRefl::operator[]";
    static constexpr char operator_deref[] = "MyDRefl::operator*";
    static constexpr char operator_ref[] = "MyDRefl::operator&";
    static constexpr char operator_member[] = "MyDRefl::operator->";
    static constexpr char operator_member_of_pointer[] = "MyDRefl::operator->*";

    static constexpr char operator_call[] = "MyDRefl::operator()";
    static constexpr char operator_comma[] = "MyDRefl::operator,";

    // container

    static constexpr char container_assign[] = "MyDRefl::container_assign";

    static constexpr char container_begin[] = "MyDRefl::container_begin";
    static constexpr char container_cbegin[] = "MyDRefl::container_cbegin";
    static constexpr char container_end[] = "MyDRefl::container_end";
    static constexpr char container_cend[] = "MyDRefl::container_cend";
    static constexpr char container_rbegin[] = "MyDRefl::container_rbegin";
    static constexpr char container_crbegin[] = "MyDRefl::container_crbegin";
    static constexpr char container_rend[] = "MyDRefl::container_rend";
    static constexpr char container_crend[] = "MyDRefl::container_crend";

    static constexpr char container_at[] = "MyDRefl::container_at";
    static constexpr char container_data[] = "MyDRefl::container_data";
    static constexpr char container_front[] = "MyDRefl::container_front";
    static constexpr char container_back[] = "MyDRefl::container_back";

    static constexpr char container_empty[] = "MyDRefl::container_empty";
    static constexpr char container_size[] = "MyDRefl::container_size";
    static constexpr char container_max_size[] = "MyDRefl::container_max_size";
    static constexpr char container_resize[] = "MyDRefl::container_resize";
    static constexpr char container_capacity[] = "MyDRefl::container_capacity";
    static constexpr char container_bucket_count[] =
        "MyDRefl::container_bucket_count";
    static constexpr char container_reserve[] = "MyDRefl::container_reserve";
    static constexpr char container_shrink_to_fit[] =
        "MyDRefl::container_shrink_to_fit";

    static constexpr char container_clear[] = "MyDRefl::container_clear";
    static constexpr char container_insert[] = "MyDRefl::container_insert";
    static constexpr char container_insert_or_assign[] =
        "MyDRefl::container_insert_or_assign";
    static constexpr char container_erase[] = "MyDRefl::container_erase";
    static constexpr char container_push_front[] =
        "MyDRefl::container_push_front";
    static constexpr char container_pop_front[] =
        "MyDRefl::container_pop_front";
    static constexpr char container_push_back[] =
        "MyDRefl::container_push_back";
    static constexpr char container_pop_back[] = "MyDRefl::container_pop_back";
    static constexpr char container_swap[] = "MyDRefl::container_swap";
    static constexpr char container_merge[] = "MyDRefl::container_merge";
    static constexpr char container_extract[] = "MyDRefl::container_extract";

    static constexpr char container_count[] = "MyDRefl::container_count";
    static constexpr char container_find[] = "MyDRefl::container_find";
    static constexpr char container_lower_bound[] =
        "MyDRefl::container_lower_bound";
    static constexpr char container_upper_bound[] =
        "MyDRefl::container_upper_bound";
    static constexpr char container_equal_range[] =
        "MyDRefl::container_equal_range";

    static constexpr char container_key_comp[] = "MyDRefl::container_key_comp";
    static constexpr char container_value_comp[] =
        "MyDRefl::container_value_comp";
    static constexpr char container_hash_function[] =
        "MyDRefl::container_hash_function";
    static constexpr char container_key_eq[] = "MyDRefl::container_key_eq";
    static constexpr char container_get_allocator[] =
        "MyDRefl::container_get_allocator";
  };

  struct MetaID {
    //
    // Global
    ///////////

    static constexpr StrID malloc{Meta::malloc};
    static constexpr StrID free{Meta::free};
    static constexpr StrID aligned_malloc{Meta::aligned_malloc};
    static constexpr StrID aligned_free{Meta::aligned_free};

    //
    // Member
    ///////////

    static constexpr StrID ctor{Meta::ctor};
    static constexpr StrID dtor{Meta::dtor};

    static constexpr StrID operator_plus{Meta::operator_plus};
    static constexpr StrID operator_minus{Meta::operator_minus};

    static constexpr StrID operator_add{Meta::operator_add};
    static constexpr StrID operator_sub{Meta::operator_sub};
    static constexpr StrID operator_mul{Meta::operator_mul};
    static constexpr StrID operator_div{Meta::operator_div};
    static constexpr StrID operator_mod{Meta::operator_mod};

    static constexpr StrID operator_bnot{Meta::operator_bnot};
    static constexpr StrID operator_band{Meta::operator_band};
    static constexpr StrID operator_bor{Meta::operator_bor};
    static constexpr StrID operator_bxor{Meta::operator_bxor};
    static constexpr StrID operator_lshift{Meta::operator_lshift};
    static constexpr StrID operator_rshift{Meta::operator_rshift};

    static constexpr StrID operator_pre_inc{Meta::operator_pre_inc};
    static constexpr StrID operator_pre_dec{Meta::operator_pre_dec};
    static constexpr StrID operator_post_inc{Meta::operator_post_inc};
    static constexpr StrID operator_post_dec{Meta::operator_post_dec};

    static constexpr StrID operator_assign{Meta::operator_assign};
    static constexpr StrID operator_assign_add{Meta::operator_assign_add};
    static constexpr StrID operator_assign_sub{Meta::operator_assign_sub};
    static constexpr StrID operator_assign_mul{Meta::operator_assign_mul};
    static constexpr StrID operator_assign_div{Meta::operator_assign_div};
    static constexpr StrID operator_assign_mod{Meta::operator_assign_mod};
    static constexpr StrID operator_assign_band{Meta::operator_assign_band};
    static constexpr StrID operator_assign_bor{Meta::operator_assign_bor};
    static constexpr StrID operator_assign_bxor{Meta::operator_assign_bxor};
    static constexpr StrID operator_assign_lshift{Meta::operator_assign_lshift};
    static constexpr StrID operator_assign_rshift{Meta::operator_assign_rshift};

    static constexpr StrID operator_eq{Meta::operator_eq};
    static constexpr StrID operator_ne{Meta::operator_ne};
    static constexpr StrID operator_lt{Meta::operator_lt};
    static constexpr StrID operator_le{Meta::operator_le};
    static constexpr StrID operator_gt{Meta::operator_gt};
    static constexpr StrID operator_ge{Meta::operator_ge};

    static constexpr StrID operator_and{Meta::operator_and};
    static constexpr StrID operator_or{Meta::operator_or};
    static constexpr StrID operator_not{Meta::operator_not};

    static constexpr StrID operator_subscript{Meta::operator_subscript};
    static constexpr StrID operator_deref{Meta::operator_deref};
    static constexpr StrID operator_ref{Meta::operator_ref};
    static constexpr StrID operator_member{Meta::operator_member};
    static constexpr StrID operator_member_of_pointer{
        Meta::operator_member_of_pointer};

    static constexpr StrID operator_call{Meta::operator_call};
    static constexpr StrID operator_comma{Meta::operator_comma};

    // container

    static constexpr StrID container_assign = Meta::container_assign;

    static constexpr StrID container_begin = Meta::container_begin;
    static constexpr StrID container_cbegin = Meta::container_cbegin;
    static constexpr StrID container_end = Meta::container_end;
    static constexpr StrID container_cend = Meta::container_cend;
    static constexpr StrID container_rbegin = Meta::container_rbegin;
    static constexpr StrID container_crbegin = Meta::container_crbegin;
    static constexpr StrID container_rend = Meta::container_rend;
    static constexpr StrID container_crend = Meta::container_crend;

    static constexpr StrID container_at = Meta::container_at;
    static constexpr StrID container_data = Meta::container_data;
    static constexpr StrID container_front = Meta::container_front;
    static constexpr StrID container_back = Meta::container_back;

    static constexpr StrID container_empty = Meta::container_empty;
    static constexpr StrID container_size = Meta::container_size;
    static constexpr StrID container_max_size = Meta::container_max_size;
    static constexpr StrID container_resize = Meta::container_resize;
    static constexpr StrID container_capacity = Meta::container_capacity;
    static constexpr StrID container_bucket_count =
        Meta::container_bucket_count;
    static constexpr StrID container_reserve = Meta::container_reserve;
    static constexpr StrID container_shrink_to_fit =
        Meta::container_shrink_to_fit;

    static constexpr StrID container_clear = Meta::container_clear;
    static constexpr StrID container_insert = Meta::container_insert;
    static constexpr StrID container_insert_or_assign =
        Meta::container_insert_or_assign;
    static constexpr StrID container_erase = Meta::container_erase;
    static constexpr StrID container_push_front = Meta::container_push_front;
    static constexpr StrID container_pop_front = Meta::container_pop_front;
    static constexpr StrID container_push_back = Meta::container_push_back;
    static constexpr StrID container_pop_back = Meta::container_pop_back;
    static constexpr StrID container_swap = Meta::container_swap;
    static constexpr StrID container_merge = Meta::container_merge;
    static constexpr StrID container_extract = Meta::container_extract;

    static constexpr StrID container_count = Meta::container_count;
    static constexpr StrID container_find = Meta::container_find;
    static constexpr StrID container_lower_bound = Meta::container_lower_bound;
    static constexpr StrID container_upper_bound = Meta::container_upper_bound;
    static constexpr StrID container_equal_range = Meta::container_equal_range;

    static constexpr StrID container_key_comp = Meta::container_key_comp;
    static constexpr StrID container_value_comp = Meta::container_value_comp;
    static constexpr StrID container_hash_function =
        Meta::container_hash_function;
    static constexpr StrID container_key_eq = Meta::container_key_eq;
    static constexpr StrID container_get_allocator =
        Meta::container_get_allocator;
  };

  using IDRegistry<StrID>::RegisterUnmanaged;
  using IDRegistry<StrID>::Register;
  using IDRegistry<StrID>::IsRegistered;

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

  using IDRegistry<TypeID>::RegisterUnmanaged;
  using IDRegistry<TypeID>::Register;
  using IDRegistry<TypeID>::IsRegistered;

  TypeIDRegistry() {
    RegisterUnmanaged(Meta::global);
    RegisterUnmanaged(Meta::t_void);
  }

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
