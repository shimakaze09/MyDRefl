#pragma once

#include "Basic.h"

//#include <string>
//#include <unordered_map>
#include <memory_resource>

#ifndef NDEBUG
#include <unordered_set>
#endif  // !NDEBUG

namespace My::MyDRefl {
// name must end with '\0'
template <typename T, typename U>
class IDRegistry {
 public:
  IDRegistry();

  void RegisterUnmanaged(T ID, std::string_view name);
  T RegisterUnmanaged(std::string_view name);
  std::string_view Register(T ID, std::string_view name);
  U Register(std::string_view name);

  bool IsRegistered(T ID) const;
  std::string_view Nameof(T ID) const;

  void UnregisterUnmanaged(T ID);
  void Clear() noexcept;

 protected:
  std::pmr::polymorphic_allocator<char> get_allocator() { return &resource; }

 private:
  std::pmr::monotonic_buffer_resource resource;
  std::pmr::unordered_map<T, std::string_view> id2name;

#ifndef NDEBUG
 public:
  bool IsUnmanaged(T ID) const;
  void ClearUnmanaged() noexcept;

 private:
  std::pmr::unordered_set<T> unmanagedIDs;
#endif  // NDEBUG
};

class NameIDRegistry : public IDRegistry<NameID, Name> {
 public:
  struct Meta {
    static constexpr Name ctor{"MyDRefl::ctor"};
    static constexpr Name dtor{"MyDRefl::dtor"};

    static constexpr Name operator_bool{"MyDRefl::operator_bool"};

    static constexpr Name operator_plus{"MyDRefl::operator+"};
    static constexpr Name operator_minus{"MyDRefl::operator-"};

    static constexpr Name operator_add{"MyDRefl::operator+"};
    static constexpr Name operator_sub{"MyDRefl::operator-"};
    static constexpr Name operator_mul{"MyDRefl::operator*"};
    static constexpr Name operator_div{"MyDRefl::operator/"};
    static constexpr Name operator_mod{"MyDRefl::operator%"};

    static constexpr Name operator_bnot{"MyDRefl::operator~"};
    static constexpr Name operator_band{"MyDRefl::operator&"};
    static constexpr Name operator_bor{"MyDRefl::operator|"};
    static constexpr Name operator_bxor{"MyDRefl::operator^"};
    static constexpr Name operator_lshift{"MyDRefl::operator<<"};
    static constexpr Name operator_rshift{"MyDRefl::operator>>"};

    static constexpr Name operator_pre_inc{"MyDRefl::operator++"};
    static constexpr Name operator_pre_dec{"MyDRefl::operator--"};
    static constexpr Name operator_post_inc{"MyDRefl::operator++"};
    static constexpr Name operator_post_dec{"MyDRefl::operator--"};

    static constexpr Name operator_assign{"MyDRefl::operator="};
    static constexpr Name operator_assign_add{"MyDRefl::operator+="};
    static constexpr Name operator_assign_sub{"MyDRefl::operator-="};
    static constexpr Name operator_assign_mul{"MyDRefl::operator*="};
    static constexpr Name operator_assign_div{"MyDRefl::operator/="};
    static constexpr Name operator_assign_mod{"MyDRefl::operator%="};
    static constexpr Name operator_assign_band{"MyDRefl::operator&="};
    static constexpr Name operator_assign_bor{"MyDRefl::operator|="};
    static constexpr Name operator_assign_bxor{"MyDRefl::operator^="};
    static constexpr Name operator_assign_lshift{"MyDRefl::operator<<="};
    static constexpr Name operator_assign_rshift{"MyDRefl::operator>>="};

    static constexpr Name operator_eq{"MyDRefl::operator=="};
    static constexpr Name operator_ne{"MyDRefl::operator!="};
    static constexpr Name operator_lt{"MyDRefl::operator<"};
    static constexpr Name operator_le{"MyDRefl::operator<="};
    static constexpr Name operator_gt{"MyDRefl::operator>"};
    static constexpr Name operator_ge{"MyDRefl::operator>="};

    static constexpr Name operator_and{"MyDRefl::operator&&"};
    static constexpr Name operator_or{"MyDRefl::operator||"};
    static constexpr Name operator_not{"MyDRefl::operator!"};

    static constexpr Name operator_subscript{"MyDRefl::operator[]"};
    static constexpr Name operator_deref{"MyDRefl::operator*"};
    static constexpr Name operator_ref{"MyDRefl::operator&"};
    static constexpr Name operator_member{"MyDRefl::operator->"};
    static constexpr Name operator_member_of_pointer{"MyDRefl::operator->*"};

    static constexpr Name operator_call{"MyDRefl::operator()"};
    static constexpr Name operator_comma{"MyDRefl::operator,"};

    // tuple

    static constexpr Name tuple_size{"MyDRefl::tuple_size"};
    static constexpr Name tuple_get{"MyDRefl::tuple_get"};

    // iterator

    static constexpr Name iterator_advance{"MyDRefl::iterator_advance"};
    static constexpr Name iterator_distance{"MyDRefl::iterator_distance"};
    static constexpr Name iterator_next{"MyDRefl::iterator_next"};
    static constexpr Name iterator_prev{"MyDRefl::iterator_prev"};

    // container

    static constexpr Name container_assign{"MyDRefl::container_assign"};

    static constexpr Name container_begin{"MyDRefl::container_begin"};
    static constexpr Name container_cbegin{"MyDRefl::container_cbegin"};
    static constexpr Name container_end{"MyDRefl::container_end"};
    static constexpr Name container_cend{"MyDRefl::container_cend"};
    static constexpr Name container_rbegin{"MyDRefl::container_rbegin"};
    static constexpr Name container_crbegin{"MyDRefl::container_crbegin"};
    static constexpr Name container_rend{"MyDRefl::container_rend"};
    static constexpr Name container_crend{"MyDRefl::container_crend"};

    static constexpr Name container_at{"MyDRefl::container_at"};
    static constexpr Name container_data{"MyDRefl::container_data"};
    static constexpr Name container_front{"MyDRefl::container_front"};
    static constexpr Name container_back{"MyDRefl::container_back"};

    static constexpr Name container_empty{"MyDRefl::container_empty"};
    static constexpr Name container_size{"MyDRefl::container_size"};
    static constexpr Name container_max_size{"MyDRefl::container_max_size"};
    static constexpr Name container_resize{"MyDRefl::container_resize"};
    static constexpr Name container_capacity{"MyDRefl::container_capacity"};
    static constexpr Name container_bucket_count{
        "MyDRefl::container_bucket_count"};
    static constexpr Name container_reserve{"MyDRefl::container_reserve"};
    static constexpr Name container_shrink_to_fit{
        "MyDRefl::container_shrink_to_fit"};

    static constexpr Name container_clear{"MyDRefl::container_clear"};
    static constexpr Name container_insert{"MyDRefl::container_insert"};
    static constexpr Name container_insert_or_assign{
        "MyDRefl::container_insert_or_assign"};
    static constexpr Name container_erase{"MyDRefl::container_erase"};
    static constexpr Name container_push_front{"MyDRefl::container_push_front"};
    static constexpr Name container_pop_front{"MyDRefl::container_pop_front"};
    static constexpr Name container_push_back{"MyDRefl::container_push_back"};
    static constexpr Name container_pop_back{"MyDRefl::container_pop_back"};
    static constexpr Name container_swap{"MyDRefl::container_swap"};
    static constexpr Name container_merge{"MyDRefl::container_merge"};
    static constexpr Name container_extract{"MyDRefl::container_extract"};

    static constexpr Name container_count{"MyDRefl::container_count"};
    static constexpr Name container_find{"MyDRefl::container_find"};
    static constexpr Name container_lower_bound{
        "MyDRefl::container_lower_bound"};
    static constexpr Name container_upper_bound{
        "MyDRefl::container_upper_bound"};
    static constexpr Name container_equal_range{
        "MyDRefl::container_equal_range"};

    static constexpr Name container_key_comp{"MyDRefl::container_key_comp"};
    static constexpr Name container_value_comp{"MyDRefl::container_value_comp"};
    static constexpr Name container_hash_function{
        "MyDRefl::container_hash_function"};
    static constexpr Name container_key_eq{"MyDRefl::container_key_eq"};
    static constexpr Name container_get_allocator{
        "MyDRefl::container_get_allocator"};
  };

  NameIDRegistry();

  using IDRegistry<NameID, Name>::Register;

  Name Register(Name n) { return Register(n.GetID(), n.GetView()); }
};

class TypeIDRegistry : public IDRegistry<TypeID, Type> {
 public:
  struct Meta {
    static constexpr Type global{"MyDRefl::global"};
    static constexpr Type t_void{"void"};
  };

  using IDRegistry<TypeID, Type>::Register;
  using IDRegistry<TypeID, Type>::IsRegistered;

  TypeIDRegistry();

  // unmanaged
  template <typename T>
  void Register();

  Type Register(Type n) { return Register(n.GetID(), n.GetName()); }

  template <typename T>
  bool IsRegistered() const;

  //
  // Type Computation
  /////////////////////

  Type RegisterAddConst(Type type);
  Type RegisterAddLValueReference(Type type);
  Type RegisterAddLValueReferenceWeak(Type type);
  Type RegisterAddRValueReference(Type type);
  Type RegisterAddConstLValueReference(Type type);
  Type RegisterAddConstRValueReference(Type type);
};
}  // namespace My::MyDRefl

#include "details/IDRegistry.inl"
