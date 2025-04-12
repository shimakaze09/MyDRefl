#include <MyDRefl/IDRegistry.h>

using namespace My;
using namespace My::MyDRefl;

NameIDRegistry::NameIDRegistry() {
  RegisterUnmanaged(Meta::ctor.GetID(), Meta::ctor.GetView());
  RegisterUnmanaged(Meta::dtor.GetID(), Meta::dtor.GetView());

  RegisterUnmanaged(Meta::operator_bool.GetID(), Meta::operator_bool.GetView());

  RegisterUnmanaged(Meta::operator_plus.GetID(), Meta::operator_plus.GetView());
  RegisterUnmanaged(Meta::operator_minus.GetID(),
                    Meta::operator_minus.GetView());

  RegisterUnmanaged(Meta::operator_add.GetID(), Meta::operator_add.GetView());
  RegisterUnmanaged(Meta::operator_sub.GetID(), Meta::operator_sub.GetView());
  RegisterUnmanaged(Meta::operator_mul.GetID(), Meta::operator_mul.GetView());
  RegisterUnmanaged(Meta::operator_div.GetID(), Meta::operator_div.GetView());
  RegisterUnmanaged(Meta::operator_mod.GetID(), Meta::operator_mod.GetView());

  RegisterUnmanaged(Meta::operator_bnot.GetID(), Meta::operator_bnot.GetView());
  RegisterUnmanaged(Meta::operator_band.GetID(), Meta::operator_band.GetView());
  RegisterUnmanaged(Meta::operator_bor.GetID(), Meta::operator_bor.GetView());
  RegisterUnmanaged(Meta::operator_bxor.GetID(), Meta::operator_bxor.GetView());
  RegisterUnmanaged(Meta::operator_lshift.GetID(),
                    Meta::operator_lshift.GetView());
  RegisterUnmanaged(Meta::operator_rshift.GetID(),
                    Meta::operator_rshift.GetView());

  RegisterUnmanaged(Meta::operator_pre_inc.GetID(),
                    Meta::operator_pre_inc.GetView());
  RegisterUnmanaged(Meta::operator_pre_dec.GetID(),
                    Meta::operator_pre_dec.GetView());
  RegisterUnmanaged(Meta::operator_post_inc.GetID(),
                    Meta::operator_post_inc.GetView());
  RegisterUnmanaged(Meta::operator_post_dec.GetID(),
                    Meta::operator_post_dec.GetView());

  RegisterUnmanaged(Meta::operator_assign.GetID(),
                    Meta::operator_assign.GetView());
  RegisterUnmanaged(Meta::operator_assign_add.GetID(),
                    Meta::operator_assign_add.GetView());
  RegisterUnmanaged(Meta::operator_assign_sub.GetID(),
                    Meta::operator_assign_sub.GetView());
  RegisterUnmanaged(Meta::operator_assign_mul.GetID(),
                    Meta::operator_assign_mul.GetView());
  RegisterUnmanaged(Meta::operator_assign_div.GetID(),
                    Meta::operator_assign_div.GetView());
  RegisterUnmanaged(Meta::operator_assign_mod.GetID(),
                    Meta::operator_assign_mod.GetView());
  RegisterUnmanaged(Meta::operator_assign_band.GetID(),
                    Meta::operator_assign_band.GetView());
  RegisterUnmanaged(Meta::operator_assign_bor.GetID(),
                    Meta::operator_assign_bor.GetView());
  RegisterUnmanaged(Meta::operator_assign_bxor.GetID(),
                    Meta::operator_assign_bxor.GetView());
  RegisterUnmanaged(Meta::operator_assign_lshift.GetID(),
                    Meta::operator_assign_lshift.GetView());
  RegisterUnmanaged(Meta::operator_assign_rshift.GetID(),
                    Meta::operator_assign_rshift.GetView());

  RegisterUnmanaged(Meta::operator_eq.GetID(), Meta::operator_eq.GetView());
  RegisterUnmanaged(Meta::operator_ne.GetID(), Meta::operator_ne.GetView());
  RegisterUnmanaged(Meta::operator_lt.GetID(), Meta::operator_lt.GetView());
  RegisterUnmanaged(Meta::operator_le.GetID(), Meta::operator_le.GetView());
  RegisterUnmanaged(Meta::operator_gt.GetID(), Meta::operator_gt.GetView());
  RegisterUnmanaged(Meta::operator_ge.GetID(), Meta::operator_ge.GetView());

  RegisterUnmanaged(Meta::operator_and.GetID(), Meta::operator_and.GetView());
  RegisterUnmanaged(Meta::operator_or.GetID(), Meta::operator_or.GetView());
  RegisterUnmanaged(Meta::operator_not.GetID(), Meta::operator_not.GetView());

  RegisterUnmanaged(Meta::operator_subscript.GetID(),
                    Meta::operator_subscript.GetView());
  RegisterUnmanaged(Meta::operator_deref.GetID(),
                    Meta::operator_deref.GetView());
  RegisterUnmanaged(Meta::operator_ref.GetID(), Meta::operator_ref.GetView());
  RegisterUnmanaged(Meta::operator_member.GetID(),
                    Meta::operator_member.GetView());
  RegisterUnmanaged(Meta::operator_member_of_pointer.GetID(),
                    Meta::operator_member_of_pointer.GetView());

  RegisterUnmanaged(Meta::operator_call.GetID(), Meta::operator_call.GetView());
  RegisterUnmanaged(Meta::operator_comma.GetID(),
                    Meta::operator_comma.GetView());

  // tuple

  RegisterUnmanaged(Meta::tuple_size.GetID(), Meta::tuple_size.GetView());
  RegisterUnmanaged(Meta::tuple_get.GetID(), Meta::tuple_get.GetView());

  // container

  RegisterUnmanaged(Meta::container_assign.GetID(),
                    Meta::container_assign.GetView());

  RegisterUnmanaged(Meta::container_begin.GetID(),
                    Meta::container_begin.GetView());
  RegisterUnmanaged(Meta::container_cbegin.GetID(),
                    Meta::container_cbegin.GetView());
  RegisterUnmanaged(Meta::container_end.GetID(), Meta::container_end.GetView());
  RegisterUnmanaged(Meta::container_cend.GetID(),
                    Meta::container_cend.GetView());
  RegisterUnmanaged(Meta::container_rbegin.GetID(),
                    Meta::container_rbegin.GetView());
  RegisterUnmanaged(Meta::container_crbegin.GetID(),
                    Meta::container_crbegin.GetView());
  RegisterUnmanaged(Meta::container_rend.GetID(),
                    Meta::container_rend.GetView());
  RegisterUnmanaged(Meta::container_crend.GetID(),
                    Meta::container_crend.GetView());

  RegisterUnmanaged(Meta::container_at.GetID(), Meta::container_at.GetView());
  RegisterUnmanaged(Meta::container_data.GetID(),
                    Meta::container_data.GetView());
  RegisterUnmanaged(Meta::container_front.GetID(),
                    Meta::container_front.GetView());
  RegisterUnmanaged(Meta::container_back.GetID(),
                    Meta::container_back.GetView());

  RegisterUnmanaged(Meta::container_empty.GetID(),
                    Meta::container_empty.GetView());
  RegisterUnmanaged(Meta::container_size.GetID(),
                    Meta::container_size.GetView());
  RegisterUnmanaged(Meta::container_max_size.GetID(),
                    Meta::container_max_size.GetView());
  RegisterUnmanaged(Meta::container_resize.GetID(),
                    Meta::container_resize.GetView());
  RegisterUnmanaged(Meta::container_capacity.GetID(),
                    Meta::container_capacity.GetView());
  RegisterUnmanaged(Meta::container_bucket_count.GetID(),
                    Meta::container_bucket_count.GetView());
  RegisterUnmanaged(Meta::container_reserve.GetID(),
                    Meta::container_reserve.GetView());
  RegisterUnmanaged(Meta::container_shrink_to_fit.GetID(),
                    Meta::container_shrink_to_fit.GetView());

  RegisterUnmanaged(Meta::container_clear.GetID(),
                    Meta::container_clear.GetView());
  RegisterUnmanaged(Meta::container_insert.GetID(),
                    Meta::container_insert.GetView());
  RegisterUnmanaged(Meta::container_insert_or_assign.GetID(),
                    Meta::container_insert_or_assign.GetView());
  RegisterUnmanaged(Meta::container_erase.GetID(),
                    Meta::container_erase.GetView());
  RegisterUnmanaged(Meta::container_push_front.GetID(),
                    Meta::container_push_front.GetView());
  RegisterUnmanaged(Meta::container_pop_front.GetID(),
                    Meta::container_pop_front.GetView());
  RegisterUnmanaged(Meta::container_push_back.GetID(),
                    Meta::container_push_back.GetView());
  RegisterUnmanaged(Meta::container_pop_back.GetID(),
                    Meta::container_pop_back.GetView());
  RegisterUnmanaged(Meta::container_swap.GetID(),
                    Meta::container_swap.GetView());
  RegisterUnmanaged(Meta::container_merge.GetID(),
                    Meta::container_merge.GetView());
  RegisterUnmanaged(Meta::container_extract.GetID(),
                    Meta::container_extract.GetView());

  RegisterUnmanaged(Meta::container_count.GetID(),
                    Meta::container_count.GetView());
  RegisterUnmanaged(Meta::container_find.GetID(),
                    Meta::container_find.GetView());
  RegisterUnmanaged(Meta::container_lower_bound.GetID(),
                    Meta::container_lower_bound.GetView());
  RegisterUnmanaged(Meta::container_upper_bound.GetID(),
                    Meta::container_upper_bound.GetView());
  RegisterUnmanaged(Meta::container_equal_range.GetID(),
                    Meta::container_equal_range.GetView());

  RegisterUnmanaged(Meta::container_key_comp.GetID(),
                    Meta::container_key_comp.GetView());
  RegisterUnmanaged(Meta::container_value_comp.GetID(),
                    Meta::container_value_comp.GetView());
  RegisterUnmanaged(Meta::container_hash_function.GetID(),
                    Meta::container_hash_function.GetView());
  RegisterUnmanaged(Meta::container_key_eq.GetID(),
                    Meta::container_key_eq.GetView());
  RegisterUnmanaged(Meta::container_get_allocator.GetID(),
                    Meta::container_get_allocator.GetView());
}

TypeIDRegistry::TypeIDRegistry() {
  RegisterUnmanaged(Meta::global.GetID(), Meta::global.GetName());
  RegisterUnmanaged(Meta::t_void.GetID(), Meta::t_void.GetName());
}

void TypeIDRegistry::RegisterUnmanaged(TypeID ID, std::string_view name) {
  assert(!type_name_is_volatile(name));
  IDRegistry<TypeID>::RegisterUnmanaged(ID, name);
}

TypeID TypeIDRegistry::RegisterUnmanaged(std::string_view name) {
  assert(!type_name_is_volatile(name));
  return IDRegistry<TypeID>::RegisterUnmanaged(name);
}

void TypeIDRegistry::Register(TypeID ID, std::string_view name) {
  if (type_name_is_const(name) || type_name_is_volatile(name)) {
    assert(false);
    return;
  }

  IDRegistry<TypeID>::Register(ID, name);
}

TypeID TypeIDRegistry::Register(std::string_view name) {
  if (type_name_is_const(name) || type_name_is_volatile(name)) {
    assert(false);
    return {};
  }

  return IDRegistry<TypeID>::Register(name);
}

//
// Type Computation
/////////////////////

Type TypeIDRegistry::RegisterAddConst(Type type) {
  std::string_view name = type.GetName();
  if (name.empty())
    return {};

  TypeID ref_ID{type_name_add_const_hash(name)};
  if (auto ref_name = Nameof(ref_ID); !ref_name.empty())
    return {ref_name, ref_ID};

  auto rst_name = type_name_add_const(name, get_allocator());

  RegisterUnmanaged(ref_ID, rst_name);
  return {rst_name, ref_ID};
}

Type TypeIDRegistry::RegisterAddLValueReference(Type type) {
  std::string_view name = type.GetName();
  if (name.empty())
    return {};

  TypeID ref_ID{type_name_add_lvalue_reference_hash(name)};
  if (auto ref_name = Nameof(ref_ID); !ref_name.empty())
    return {ref_name, ref_ID};

  auto rst_name = type_name_add_lvalue_reference(name, get_allocator());

  RegisterUnmanaged(ref_ID, rst_name);
  return {rst_name, ref_ID};
}

Type TypeIDRegistry::RegisterAddLValueReferenceWeak(Type type) {
  std::string_view name = type.GetName();
  if (name.empty())
    return {};

  TypeID ref_ID{type_name_add_lvalue_reference_weak_hash(name)};
  if (auto ref_name = Nameof(ref_ID); !ref_name.empty())
    return {ref_name, ref_ID};

  auto rst_name = type_name_add_lvalue_reference_weak(name, get_allocator());

  RegisterUnmanaged(ref_ID, rst_name);

  return {rst_name, ref_ID};
}

Type TypeIDRegistry::RegisterAddRValueReference(Type type) {
  std::string_view name = type.GetName();
  if (name.empty())
    return {};

  TypeID ref_ID{type_name_add_rvalue_reference_hash(name)};
  if (auto ref_name = Nameof(ref_ID); !ref_name.empty())
    return {ref_name, ref_ID};

  auto rst_name = type_name_add_rvalue_reference(name, get_allocator());

  RegisterUnmanaged(ref_ID, rst_name);

  return {rst_name, ref_ID};
}

Type TypeIDRegistry::RegisterAddConstLValueReference(Type type) {
  std::string_view name = type.GetName();
  if (name.empty())
    return {};

  TypeID ref_ID{type_name_add_const_lvalue_reference_hash(name)};
  if (auto ref_name = Nameof(ref_ID); !ref_name.empty())
    return {ref_name, ref_ID};

  auto rst_name = type_name_add_const_lvalue_reference(name, get_allocator());

  RegisterUnmanaged(ref_ID, rst_name);

  return {rst_name, ref_ID};
}

Type TypeIDRegistry::RegisterAddConstRValueReference(Type type) {
  std::string_view name = type.GetName();
  if (name.empty())
    return {};

  TypeID ref_ID{type_name_add_const_rvalue_reference_hash(name)};
  if (auto ref_name = Nameof(ref_ID); !ref_name.empty())
    return {ref_name, ref_ID};

  auto rst_name = type_name_add_const_rvalue_reference(name, get_allocator());

  RegisterUnmanaged(ref_ID, rst_name);
  return {rst_name, ref_ID};
}
