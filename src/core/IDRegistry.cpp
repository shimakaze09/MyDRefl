#include <MyDRefl/IDRegistry.hpp>

using namespace Smkz;
using namespace Smkz::MyDRefl;

#define MY_MYDREFL_META_GET_ID_VIEW(meta) meta.GetID(), meta.GetView()
#define MY_MYDREFL_META_GET_ID_NAME(meta) meta.GetID(), meta.GetName()

NameIDRegistry::NameIDRegistry() {
  // operators
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::operator_bool));

  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::operator_add));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::operator_sub));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::operator_mul));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::operator_div));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::operator_mod));

  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::operator_bnot));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::operator_band));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::operator_bor));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::operator_bxor));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::operator_shl));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::operator_shr));

  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::operator_pre_inc));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::operator_pre_dec));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::operator_post_inc));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::operator_post_dec));

  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::operator_assignment));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::operator_assignment_add));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::operator_assignment_sub));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::operator_assignment_mul));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::operator_assignment_div));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::operator_assignment_mod));
  RegisterUnmanaged(
      MY_MYDREFL_META_GET_ID_VIEW(Meta::operator_assignment_band));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::operator_assignment_bor));
  RegisterUnmanaged(
      MY_MYDREFL_META_GET_ID_VIEW(Meta::operator_assignment_bxor));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::operator_assignment_shl));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::operator_assignment_shr));

  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::operator_eq));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::operator_ne));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::operator_lt));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::operator_le));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::operator_gt));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::operator_ge));

  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::operator_and));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::operator_or));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::operator_not));

  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::operator_subscript));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::operator_indirection));

  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::operator_call));

  // non-member functions

  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::ctor));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::dtor));

  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::get));

  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::tuple_size));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::tuple_element));

  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::holds_alternative));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::get_if));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::variant_size));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::variant_alternative));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::variant_visit_get));

  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::advance));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::distance));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::next));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::prev));

  // member functions

  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_assign));

  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_begin));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_cbegin));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_end));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_cend));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_rbegin));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_crbegin));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_rend));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_crend));

  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_at));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_data));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_front));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_back));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_top));

  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_empty));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_size));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_size_bytes));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_resize));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_capacity));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_bucket_count));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_reserve));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_shrink_to_fit));

  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_clear));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_insert));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_insert_after));
  RegisterUnmanaged(
      MY_MYDREFL_META_GET_ID_VIEW(Meta::container_insert_or_assign));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_erase));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_erase_after));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_push_front));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_pop_front));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_push_back));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_pop_back));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_push));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_pop));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_swap));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_merge));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_extract));

  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_splice_after));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_splice));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_remove));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_reverse));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_unique));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_sort));

  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_count));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_find));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_lower_bound));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_upper_bound));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::container_equal_range));

  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::variant_index));
  RegisterUnmanaged(
      MY_MYDREFL_META_GET_ID_VIEW(Meta::variant_valueless_by_exception));

  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::optional_has_value));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::optional_value));
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_VIEW(Meta::optional_reset));
}

Name NameIDRegistry::Nameof(NameID ID) const {
  auto view = Viewof(ID);
  if (view.empty()) return {};
  return {view, ID};
}

TypeIDRegistry::TypeIDRegistry() {
  RegisterUnmanaged(MY_MYDREFL_META_GET_ID_NAME(Meta::global));
}

Type TypeIDRegistry::Typeof(TypeID ID) const {
  auto view = Viewof(ID);
  if (view.empty()) return {};
  return {view, ID};
}

//
// Type Computation
/////////////////////

Type TypeIDRegistry::RegisterAddConst(Type type) {
  std::string_view name = type.GetName();
  if (name.empty()) return {};

  TypeID ref_ID{type_name_add_const_hash(name)};
  if (auto ref_name = Viewof(ref_ID); !ref_name.empty())
    return {ref_name, ref_ID};

  std::string_view rst_name;
  {
    std::lock_guard wlock{smutex};  // write resource
    rst_name = type_name_add_const(name, get_allocator());
  }

  RegisterUnmanaged(ref_ID, rst_name);
  return {rst_name, ref_ID};
}

Type TypeIDRegistry::RegisterAddLValueReference(Type type) {
  std::string_view name = type.GetName();
  if (name.empty()) return {};

  TypeID ref_ID{type_name_add_lvalue_reference_hash(name)};
  if (auto ref_name = Viewof(ref_ID); !ref_name.empty())
    return {ref_name, ref_ID};

  std::string_view rst_name;
  {
    std::lock_guard wlock{smutex};  // write resource
    rst_name = type_name_add_lvalue_reference(name, get_allocator());
  }

  RegisterUnmanaged(ref_ID, rst_name);
  return {rst_name, ref_ID};
}

Type TypeIDRegistry::RegisterAddLValueReferenceWeak(Type type) {
  std::string_view name = type.GetName();
  if (name.empty()) return {};

  TypeID ref_ID{type_name_add_lvalue_reference_weak_hash(name)};
  if (auto ref_name = Viewof(ref_ID); !ref_name.empty())
    return {ref_name, ref_ID};

  std::string_view rst_name;
  {
    std::lock_guard wlock{smutex};  // write resource
    rst_name = type_name_add_lvalue_reference_weak(name, get_allocator());
  }

  RegisterUnmanaged(ref_ID, rst_name);

  return {rst_name, ref_ID};
}

Type TypeIDRegistry::RegisterAddRValueReference(Type type) {
  std::string_view name = type.GetName();
  if (name.empty()) return {};

  TypeID ref_ID{type_name_add_rvalue_reference_hash(name)};
  if (auto ref_name = Viewof(ref_ID); !ref_name.empty())
    return {ref_name, ref_ID};

  std::string_view rst_name;
  {
    std::lock_guard wlock{smutex};  // write resource
    rst_name = type_name_add_rvalue_reference(name, get_allocator());
  }

  RegisterUnmanaged(ref_ID, rst_name);

  return {rst_name, ref_ID};
}

Type TypeIDRegistry::RegisterAddConstLValueReference(Type type) {
  std::string_view name = type.GetName();
  if (name.empty()) return {};

  TypeID ref_ID{type_name_add_const_lvalue_reference_hash(name)};
  if (auto ref_name = Viewof(ref_ID); !ref_name.empty())
    return {ref_name, ref_ID};

  std::string_view rst_name;
  {
    std::lock_guard wlock{smutex};  // write resource
    rst_name = type_name_add_const_lvalue_reference(name, get_allocator());
  }

  RegisterUnmanaged(ref_ID, rst_name);

  return {rst_name, ref_ID};
}

Type TypeIDRegistry::RegisterAddConstRValueReference(Type type) {
  std::string_view name = type.GetName();
  if (name.empty()) return {};

  TypeID ref_ID{type_name_add_const_rvalue_reference_hash(name)};
  if (auto ref_name = Viewof(ref_ID); !ref_name.empty())
    return {ref_name, ref_ID};

  std::string_view rst_name;
  {
    std::lock_guard wlock{smutex};  // write resource
    rst_name = type_name_add_const_rvalue_reference(name, get_allocator());
  }

  RegisterUnmanaged(ref_ID, rst_name);
  return {rst_name, ref_ID};
}

#undef MY_MYDREFL_META_GET_ID_VIEW
#undef MY_MYDREFL_META_GET_ID_NAME
