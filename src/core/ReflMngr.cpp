#include <MyDRefl/ReflMngr.h>

#include "InvokeUtil.h"

#include "ReflMngrInitUtil/ReflMngrInitUtil.h"

#include <string>

using namespace My;
using namespace My::MyDRefl;

namespace My::MyDRefl::details {
static ObjectView StaticCast_DerivedToBase(ObjectView obj, Type type) {
  assert(obj.GetType().GetCVRefMode() == CVRefMode::None);

  if (obj.GetType() == type)
    return obj;

  auto target = Mngr.typeinfos.find(obj.GetType());
  if (target == Mngr.typeinfos.end())
    return {};

  const auto& typeinfo = target->second;

  for (const auto& [base, baseinfo] : typeinfo.baseinfos) {
    auto ptr = StaticCast_DerivedToBase(
        ObjectView{base, baseinfo.StaticCast_DerivedToBase(obj.GetPtr())},
        type);
    if (ptr.GetType())
      return ptr;
  }

  return {};
}

static ObjectView StaticCast_BaseToDerived(ObjectView obj, Type type) {
  assert(obj.GetType().GetCVRefMode() == CVRefMode::None);

  if (obj.GetType() == type)
    return obj;

  auto target = Mngr.typeinfos.find(type);
  if (target == Mngr.typeinfos.end())
    return {};

  const auto& typeinfo = target->second;

  for (const auto& [base, baseinfo] : typeinfo.baseinfos) {
    auto ptr = StaticCast_BaseToDerived(obj, base);
    if (ptr.GetType())
      return {base, baseinfo.IsVirtual()
                        ? nullptr
                        : baseinfo.StaticCast_BaseToDerived(obj.GetPtr())};
  }

  return {};
}

static ObjectView DynamicCast_BaseToDerived(ObjectView obj, Type type) {
  assert(obj.GetType().GetCVRefMode() == CVRefMode::None);

  if (obj.GetType() == type)
    return obj;

  auto target = Mngr.typeinfos.find(obj.GetType());
  if (target == Mngr.typeinfos.end())
    return {};

  const auto& typeinfo = target->second;

  for (const auto& [base, baseinfo] : typeinfo.baseinfos) {
    auto ptr = DynamicCast_BaseToDerived(
        ObjectView{base, baseinfo.DynamicCast_BaseToDerived(obj.GetPtr())},
        type);
    if (ptr.GetType())
      return {base, baseinfo.IsPolymorphic()
                        ? baseinfo.DynamicCast_BaseToDerived(obj.GetPtr())
                        : nullptr};
  }

  return {};
}

static ObjectView Var(ObjectView obj, Name field_name, FieldFlag flag) {
  assert(obj.GetType().GetCVRefMode() == CVRefMode::None);

  auto ttarget = Mngr.typeinfos.find(obj.GetType());
  if (ttarget == Mngr.typeinfos.end())
    return {};

  auto& typeinfo = ttarget->second;

  auto ftarget = typeinfo.fieldinfos.find(field_name);
  if (ftarget != typeinfo.fieldinfos.end() &&
      enum_contain(flag, ftarget->second.fieldptr.GetFieldFlag()))
    return ftarget->second.fieldptr.Var(obj.GetPtr());

  for (const auto& [base, baseinfo] : typeinfo.baseinfos) {
    auto bptr =
        Var(ObjectView{base, baseinfo.StaticCast_DerivedToBase(obj.GetPtr())},
            field_name, flag);
    if (bptr.GetType())
      return bptr;
  }

  return {};
}

static Type IsInvocable(bool is_priority, Type type, Name method_name,
                        std::span<const Type> argTypes, MethodFlag flag) {
  assert(type.GetCVRefMode() == CVRefMode::None);
  auto typetarget = Mngr.typeinfos.find(type);

  if (typetarget == Mngr.typeinfos.end())
    return {};

  const auto& typeinfo = typetarget->second;
  auto [begin_iter, end_iter] = typeinfo.methodinfos.equal_range(method_name);

  // 1. object variable and static
  if (enum_contain(flag, MethodFlag::Priority)) {
    for (auto iter = begin_iter; iter != end_iter; ++iter) {
      if (enum_contain(MethodFlag::Priority,
                       iter->second.methodptr.GetMethodFlag()) &&
          (is_priority
               ? IsPriorityCompatible(iter->second.methodptr.GetParamList(),
                                      argTypes)
               : Mngr.IsCompatible(iter->second.methodptr.GetParamList(),
                                   argTypes))) {
        return iter->second.methodptr.GetResultType();
      }
    }
  }

  // 2. object const
  if (enum_contain(flag, MethodFlag::Const)) {
    for (auto iter = begin_iter; iter != end_iter; ++iter) {
      if (iter->second.methodptr.GetMethodFlag() == MethodFlag::Const &&
          (is_priority
               ? IsPriorityCompatible(iter->second.methodptr.GetParamList(),
                                      argTypes)
               : Mngr.IsCompatible(iter->second.methodptr.GetParamList(),
                                   argTypes))) {
        return iter->second.methodptr.GetResultType();
      }
    }
  }

  for (const auto& [base, baseinfo] : typeinfo.baseinfos) {
    if (auto rst = IsInvocable(is_priority, base, method_name, argTypes, flag))
      return rst;
  }

  return {};
}

static Type BInvoke(bool is_priority, std::pmr::memory_resource* args_rsrc,
                    ObjectView obj, Name method_name, void* result_buffer,
                    ArgsView args, MethodFlag flag) {
  assert(obj.GetType().GetCVRefMode() == CVRefMode::None);

  auto typetarget = Mngr.typeinfos.find(obj.GetType());

  if (typetarget == Mngr.typeinfos.end())
    return {};

  const auto& typeinfo = typetarget->second;

  auto [begin_iter, end_iter] = typeinfo.methodinfos.equal_range(method_name);

  if (enum_contain_any(flag, MethodFlag::Priority)) {
    for (auto iter = begin_iter; iter != end_iter; ++iter) {
      if (enum_contain_any(flag, iter->second.methodptr.GetMethodFlag())) {
        NewArgsGuard guard{is_priority, args_rsrc,
                           iter->second.methodptr.GetParamList(), args};
        if (!guard.IsCompatible())
          continue;
        iter->second.methodptr.Invoke(obj.GetPtr(), result_buffer,
                                      guard.GetArgsView());
        return iter->second.methodptr.GetResultType();
      }
    }
  }
  if (enum_contain_any(flag, MethodFlag::Const)) {
    for (auto iter = begin_iter; iter != end_iter; ++iter) {
      if (iter->second.methodptr.GetMethodFlag() == MethodFlag::Const) {
        NewArgsGuard guard{is_priority, args_rsrc,
                           iter->second.methodptr.GetParamList(), args};
        if (!guard.IsCompatible())
          continue;
        iter->second.methodptr.Invoke(static_cast<const void*>(obj.GetPtr()),
                                      result_buffer, guard.GetArgsView());
        return iter->second.methodptr.GetResultType();
      }
    }
  }

  for (const auto& [base, baseinfo] : typeinfo.baseinfos) {
    auto rst = BInvoke(
        is_priority, args_rsrc,
        ObjectView{base, baseinfo.StaticCast_DerivedToBase(obj.GetPtr())},
        method_name, result_buffer, args, flag);
    if (rst)
      return rst;
  }

  return {};
}

static SharedObject MInvoke(bool is_priority,
                            std::pmr::memory_resource* args_rsrc,
                            ObjectView obj, Name method_name,
                            std::pmr::memory_resource* rst_rsrc, ArgsView args,
                            MethodFlag flag) {
  assert(args_rsrc);
  assert(rst_rsrc);
  assert(obj.GetType().GetCVRefMode() == CVRefMode::None);
  auto typetarget = Mngr.typeinfos.find(obj.GetType());

  if (typetarget == Mngr.typeinfos.end())
    return {};

  const auto& typeinfo = typetarget->second;

  auto [begin_iter, end_iter] = typeinfo.methodinfos.equal_range(method_name);

  if (enum_contain_any(flag, MethodFlag::Priority)) {
    for (auto iter = begin_iter; iter != end_iter; ++iter) {
      if (enum_contain(flag, iter->second.methodptr.GetMethodFlag())) {
        NewArgsGuard guard{is_priority, args_rsrc,
                           iter->second.methodptr.GetParamList(), args};

        if (!guard.IsCompatible())
          continue;

        const auto& methodptr = iter->second.methodptr;
        const auto& rst_type = methodptr.GetResultType();

        if (rst_type.Is<void>()) {
          iter->second.methodptr.Invoke(obj.GetPtr(), nullptr,
                                        guard.GetArgsView());
          return SharedObject{Type_of<void>};
        } else if (rst_type.IsReference()) {
          std::aligned_storage_t<sizeof(void*)> buffer;
          iter->second.methodptr.Invoke(obj.GetPtr(), &buffer,
                                        guard.GetArgsView());
          return {rst_type, buffer_as<void*>(&buffer)};
        } else if (rst_type.Is<ObjectView>()) {
          std::aligned_storage_t<sizeof(ObjectView)> buffer;
          iter->second.methodptr.Invoke(obj.GetPtr(), &buffer,
                                        guard.GetArgsView());
          return SharedObject{buffer_as<ObjectView>(&buffer)};
        } else if (rst_type.Is<SharedObject>()) {
          SharedObject buffer;
          iter->second.methodptr.Invoke(obj.GetPtr(), &buffer,
                                        guard.GetArgsView());
          return buffer;
        } else {
          if (!Mngr.IsDestructible(rst_type))
            return {};
          auto* result_typeinfo = Mngr.GetTypeInfo(rst_type);
          if (!result_typeinfo)
            return {};
          void* result_buffer = rst_rsrc->allocate(result_typeinfo->size,
                                                   result_typeinfo->alignment);
          iter->second.methodptr.Invoke(obj.GetPtr(), result_buffer,
                                        guard.GetArgsView());
          return {{rst_type, result_buffer}, [rst_type, rst_rsrc](void* ptr) {
                    Mngr.MDelete({rst_type, ptr}, rst_rsrc);
                  }};
        }
      }
    }
  }

  if (enum_contain_any(flag, MethodFlag::Const)) {
    for (auto iter = begin_iter; iter != end_iter; ++iter) {
      if (iter->second.methodptr.GetMethodFlag() == MethodFlag::Const) {
        NewArgsGuard guard{is_priority, args_rsrc,
                           iter->second.methodptr.GetParamList(), args};

        if (!guard.IsCompatible())
          continue;

        const auto& methodptr = iter->second.methodptr;
        const auto& rst_type = methodptr.GetResultType();

        if (rst_type.Is<void>()) {
          iter->second.methodptr.Invoke(static_cast<const void*>(obj.GetPtr()),
                                        nullptr, guard.GetArgsView());
          return SharedObject{rst_type};
        } else if (rst_type.IsReference()) {
          std::aligned_storage_t<sizeof(void*)> buffer;
          iter->second.methodptr.Invoke(static_cast<const void*>(obj.GetPtr()),
                                        &buffer, guard.GetArgsView());
          return {rst_type, buffer_as<void*>(&buffer)};
        } else if (rst_type.Is<ObjectView>()) {
          std::aligned_storage_t<sizeof(ObjectView)> buffer;
          iter->second.methodptr.Invoke(obj.GetPtr(), &buffer,
                                        guard.GetArgsView());
          return SharedObject{buffer_as<ObjectView>(&buffer)};
        } else if (rst_type.Is<SharedObject>()) {
          SharedObject buffer;
          iter->second.methodptr.Invoke(obj.GetPtr(), &buffer,
                                        guard.GetArgsView());
          return buffer;
        } else {
          if (!Mngr.IsDestructible(rst_type))
            return {};
          auto* result_typeinfo = Mngr.GetTypeInfo(rst_type);
          if (!result_typeinfo)
            return {};
          void* result_buffer = rst_rsrc->allocate(result_typeinfo->size,
                                                   result_typeinfo->alignment);
          iter->second.methodptr.Invoke(static_cast<const void*>(obj.GetPtr()),
                                        result_buffer, guard.GetArgsView());
          return {{rst_type, result_buffer}, [rst_type, rst_rsrc](void* ptr) {
                    Mngr.MDelete({rst_type, ptr}, rst_rsrc);
                  }};
        }
      }
    }
  }

  for (const auto& [base, baseinfo] : typeinfo.baseinfos) {
    auto rst = MInvoke(
        is_priority, args_rsrc,
        ObjectView{base, baseinfo.StaticCast_DerivedToBase(obj.GetPtr())},
        method_name, rst_rsrc, args, flag);
    if (rst.GetType())
      return rst;
  }

  return {};
}

static bool ForEachTypeInfo(Type type,
                            const std::function<bool(InfoTypePair)>& func,
                            std::set<TypeID>& visitedVBs) {
  assert(type.GetCVRefMode() == CVRefMode::None);
  auto target = Mngr.typeinfos.find(type);

  if (target == Mngr.typeinfos.end())
    return true;

  auto& typeinfo = target->second;

  if (!func({type, &typeinfo}))
    return false;

  for (auto& [base, baseinfo] : typeinfo.baseinfos) {
    if (baseinfo.IsVirtual()) {
      if (visitedVBs.find(base) != visitedVBs.end())
        continue;
      visitedVBs.insert(base);
    }

    if (!ForEachTypeInfo(base, func, visitedVBs))
      return false;
  }

  return true;
}

static bool ForEachVar(
    ObjectView obj,
    const std::function<bool(InfoTypePair, InfoFieldPair, ObjectView)>& func,
    FieldFlag flag, std::set<TypeID>& visitedVBs) {
  assert(obj.GetType().GetCVRefMode() == CVRefMode::None);

  auto target = Mngr.typeinfos.find(obj.GetType());

  if (target == Mngr.typeinfos.end())
    return true;

  auto& typeinfo = target->second;

  for (auto& [field, fieldInfo] : typeinfo.fieldinfos) {
    if (!enum_contain(flag, fieldInfo.fieldptr.GetFieldFlag()))
      continue;

    if (!func({obj.GetType(), &typeinfo}, {field, &fieldInfo},
              fieldInfo.fieldptr.Var(obj.GetPtr())))
      return false;
  }

  for (const auto& [base, baseinfo] : typeinfo.baseinfos) {
    if (baseinfo.IsVirtual()) {
      if (visitedVBs.find(base) != visitedVBs.end())
        continue;
      visitedVBs.insert(base);
    }

    if (!ForEachVar(
            ObjectView{base, baseinfo.StaticCast_DerivedToBase(obj.GetPtr())},
            func, flag, visitedVBs))
      return false;
  }

  return true;
}

static bool ContainsField(Type type, Name field_name, FieldFlag flag) {
  assert(type.GetCVRefMode() == CVRefMode::None);

  auto ttarget = Mngr.typeinfos.find(type);
  if (ttarget == Mngr.typeinfos.end())
    return false;

  auto& typeinfo = ttarget->second;

  auto ftarget = typeinfo.fieldinfos.find(field_name);
  if (ftarget != typeinfo.fieldinfos.end() &&
      enum_contain(flag, ftarget->second.fieldptr.GetFieldFlag()))
    return true;

  for (const auto& [basetype, baseinfo] : typeinfo.baseinfos) {
    if (ContainsField(basetype, field_name, flag))
      return true;
  }
  return false;
}

static bool ContainsMethod(Type type, Name method_name, MethodFlag flag) {
  assert(type.GetCVRefMode() == CVRefMode::None);

  auto ttarget = Mngr.typeinfos.find(type);
  if (ttarget == Mngr.typeinfos.end())
    return false;

  auto& typeinfo = ttarget->second;

  auto [begin_iter, end_iter] = typeinfo.methodinfos.equal_range(method_name);
  for (auto iter = begin_iter; iter != end_iter; ++iter) {
    if (enum_contain(flag, iter->second.methodptr.GetMethodFlag()))
      return true;
  }

  for (const auto& [basetype, baseinfo] : typeinfo.baseinfos) {
    if (ContainsMethod(basetype, method_name, flag))
      return true;
  }

  return false;
}
}  // namespace My::MyDRefl::details

ReflMngr::ReflMngr() {
  RegisterType(GlobalType, 0, 1, false, true);

  details::ReflMngrInitUtil_0(*this);
  details::ReflMngrInitUtil_1(*this);
  details::ReflMngrInitUtil_2(*this);
  details::ReflMngrInitUtil_3(*this);
  details::ReflMngrInitUtil_4(*this);
  details::ReflMngrInitUtil_5(*this);
  details::ReflMngrInitUtil_6(*this);
}

TypeInfo* ReflMngr::GetTypeInfo(Type type) const {
  auto target = typeinfos.find(type.RemoveCVRef());
  if (target == typeinfos.end())
    return nullptr;
  return const_cast<TypeInfo*>(&target->second);
}

void ReflMngr::Clear() noexcept {
  // field attrs
  for (auto& [type, typeinfo] : typeinfos) {
    for (auto& [field, fieldinfo] : typeinfo.fieldinfos)
      fieldinfo.attrs.clear();
  }

  // type attrs
  for (auto& [ID, typeinfo] : typeinfos)
    typeinfo.attrs.clear();

  // type dynamic field
  for (auto& [type, typeinfo] : typeinfos) {
    auto iter = typeinfo.fieldinfos.begin();
    while (iter != typeinfo.fieldinfos.end()) {
      auto cur = iter;
      ++iter;

      if (cur->second.fieldptr.GetFieldFlag() == FieldFlag::DynamicShared)
        typeinfo.fieldinfos.erase(cur);
    }
  }

  typeinfos.clear();

  temporary_resource.release();
  object_resource.release();
}

ReflMngr::~ReflMngr() {
  Clear();
}

bool ReflMngr::ContainsVirtualBase(Type type) const {
  auto* info = GetTypeInfo(type);
  if (!info)
    return false;

  for (const auto& [base, baseinfo] : info->baseinfos) {
    if (baseinfo.IsVirtual() || ContainsVirtualBase(base))
      return true;
  }

  return false;
}

Type ReflMngr::RegisterType(Type type, size_t size, size_t alignment,
                            bool is_polymorphic, bool is_trivial) {
  assert(alignment > 0 && (alignment & (alignment - 1)) == 0);
  auto target = typeinfos.find(type.RemoveCVRef());
  if (target != typeinfos.end())
    return {};
  Type new_type = {tregistry.Register(type.GetID(), type.GetName()),
                   type.GetID()};
  typeinfos.emplace_hint(target, new_type,
                         TypeInfo{size, alignment, is_polymorphic, is_trivial});
  if (is_trivial)
    AddTrivialCopyConstructor(type);
  return new_type;
}

Type ReflMngr::RegisterType(Type type, std::span<const Type> bases,
                            std::span<const Type> field_types,
                            std::span<const Name> field_names,
                            bool is_trivial) {
  assert(field_types.size() == field_names.size());

  if (typeinfos.contains(type))
    return {};

  std::size_t size = 0;
  std::size_t alignment = 1;

  const size_t num_field = field_types.size();

  std::pmr::vector<std::size_t> base_offsets{&temporary_resource};
  base_offsets.resize(bases.size());

  for (size_t i = 0; i < bases.size(); i++) {
    const auto& basetype = bases[i];
    auto btarget = typeinfos.find(basetype);
    if (btarget == typeinfos.end())
      return {};
    const auto& baseinfo = btarget->second;
    if (baseinfo.is_polymorphic || ContainsVirtualBase(basetype))
      return {};
    if (!baseinfo.is_trivial)
      is_trivial = false;
    assert(baseinfo.alignment > 0 &&
           (baseinfo.alignment & (baseinfo.alignment - 1)) == 0);
    size = (size + (baseinfo.alignment - 1)) & ~(baseinfo.alignment - 1);
    base_offsets[i] = size;
    size += baseinfo.size;
    if (baseinfo.alignment > alignment)
      alignment = baseinfo.alignment;
  }

  std::pmr::vector<std::size_t> field_offsets{&temporary_resource};
  field_offsets.resize(num_field);

  for (size_t i = 0; i < num_field; ++i) {
    const auto& field_type = field_types[i];
    auto fttarget = typeinfos.find(field_type);
    if (fttarget == typeinfos.end())
      return {};
    const auto& ftinfo = fttarget->second;
    if (!ftinfo.is_trivial)
      is_trivial = false;
    assert(ftinfo.alignment > 0 &&
           (ftinfo.alignment & (ftinfo.alignment - 1)) == 0);
    size = (size + (ftinfo.alignment - 1)) & ~(ftinfo.alignment - 1);
    field_offsets[i] = size;
    size += ftinfo.size;
    if (ftinfo.alignment > alignment)
      alignment = ftinfo.alignment;
  }

  size = (size + (alignment - 1)) & ~(alignment - 1);

  Type newtype = RegisterType(type, size, alignment, false, is_trivial);
  for (size_t i = 0; i < bases.size(); i++) {
    AddBase(type, bases[i],
            BaseInfo{{[offset = base_offsets[i]](void* derived) {
                        return forward_offset(derived, offset);
                      },
                      [offset = base_offsets[i]](void* base) {
                        return backward_offset(base, offset);
                      }}});
  }

  for (size_t i = 0; i < num_field; ++i) {
    AddField(type, field_names[i],
             FieldInfo{{field_types[i], field_offsets[i]}});
  }

  return newtype;
}

Name ReflMngr::AddField(Type type, Name field_name, FieldInfo fieldinfo) {
  auto* typeinfo = GetTypeInfo(type);
  if (!typeinfo)
    return {};
  auto ftarget = typeinfo->fieldinfos.find(field_name);
  if (ftarget != typeinfo->fieldinfos.end())
    return {};

  Name new_field_name = {
      nregistry.Register(field_name.GetID(), field_name.GetView()),
      field_name.GetID()};
  typeinfo->fieldinfos.emplace_hint(ftarget, new_field_name,
                                    std::move(fieldinfo));

  return new_field_name;
}

Name ReflMngr::AddMethod(Type type, Name method_name, MethodInfo methodinfo) {
  auto* typeinfo = GetTypeInfo(type);
  if (!typeinfo)
    return {};

  auto [begin_iter, end_iter] = typeinfo->methodinfos.equal_range(method_name);
  for (auto iter = begin_iter; iter != end_iter; ++iter) {
    if (!iter->second.methodptr.IsDistinguishableWith(methodinfo.methodptr))
      return {};
  }
  Name new_method_name = {
      nregistry.Register(method_name.GetID(), method_name.GetView()),
      method_name.GetID()};
  typeinfo->methodinfos.emplace(new_method_name, std::move(methodinfo));
  return new_method_name;
}

Name ReflMngr::AddTrivialDefaultConstructor(Type type) {
  auto target = typeinfos.find(type);
  if (target == typeinfos.end() || target->second.is_polymorphic ||
      ContainsVirtualBase(type))
    return {};
  for (const auto& [basetype, baseinfo] : target->second.baseinfos) {
    assert(
        !baseinfo
             .IsPolymorphic());  // type isn't polymorphic => bases aren't polymorphic
    if (baseinfo.IsVirtual())
      return {};
  }
  return AddMethod(
      type, NameIDRegistry::Meta::ctor,
      MethodInfo{{[](void* obj, void*, ArgsView) {}, MethodFlag::Variable}});
}

Name ReflMngr::AddTrivialCopyConstructor(Type type) {
  auto target = typeinfos.find(type);
  if (target == typeinfos.end())
    return {};
  auto& typeinfo = target->second;
  return AddMethod(
      type, NameIDRegistry::Meta::ctor,
      MethodInfo{{
          [size = typeinfo.size](void* obj, void*, ArgsView args) {
            memcpy(obj, args[0].GetPtr(), size);
          },
          MethodFlag::Variable,
          {},                                                // result type
          {tregistry.RegisterAddConstLValueReference(type)}  // paramlist
      }});
}

Name ReflMngr::AddZeroDefaultConstructor(Type type) {
  auto target = typeinfos.find(type);
  if (target == typeinfos.end() || target->second.is_polymorphic ||
      ContainsVirtualBase(type))
    return {};
  for (const auto& [basetype, baseinfo] : target->second.baseinfos) {
    assert(
        !baseinfo
             .IsPolymorphic());  // type isn't polymorphic => bases aren't polymorphic
    if (baseinfo.IsVirtual())
      return {};
  }
  return AddMethod(
      type, NameIDRegistry::Meta::ctor,
      MethodInfo{{[size = target->second.size](void* obj, void*, ArgsView) {
                    std::memset(obj, 0, size);
                  },
                  MethodFlag::Variable}});
}

Name ReflMngr::AddDefaultConstructor(Type type) {
  if (IsConstructible(type))
    return {};

  auto target = typeinfos.find(type);
  if (target == typeinfos.end() || target->second.is_polymorphic ||
      ContainsVirtualBase(type))
    return {};
  const auto& typeinfo = target->second;
  for (const auto& [basetype, baseinfo] : typeinfo.baseinfos) {
    assert(
        !baseinfo.IsPolymorphic() &&
        !baseinfo
             .IsVirtual());  // type isn't polymorphic => bases aren't polymorphic
    if (!IsConstructible(basetype))
      return {};
  }
  for (const auto& [fieldname, fieldinfo] : typeinfo.fieldinfos) {
    if (fieldinfo.fieldptr.GetFieldFlag() == FieldFlag::Unowned)
      continue;

    if (!IsConstructible(fieldinfo.fieldptr.GetType()))
      return {};
  }
  const auto& t = target->first;
  return AddMethod(
      type, NameIDRegistry::Meta::ctor,
      MethodInfo{
          {[t](void* obj, void*, ArgsView) {
             const auto& typeinfo = Mngr.typeinfos.at(t);
             for (const auto& [basetype, baseinfo] : typeinfo.baseinfos) {
               void* baseptr = baseinfo.StaticCast_DerivedToBase(obj);
               bool success = Mngr.Construct(ObjectView{basetype, baseptr});
               assert(success);
             }

             for (const auto& [fieldname, fieldinfo] : typeinfo.fieldinfos) {
               if (fieldinfo.fieldptr.GetFieldFlag() == FieldFlag::Unowned)
                 continue;

               if (fieldinfo.fieldptr.GetType().IsPointer())
                 buffer_as<void*>(fieldinfo.fieldptr.Var(obj).GetPtr()) =
                     nullptr;
               else
                 Mngr.Construct(fieldinfo.fieldptr.Var(obj));
             }
           },
           MethodFlag::Variable}});
}

Name ReflMngr::AddDestructor(Type type) {
  if (IsDestructible(type))
    return {};

  auto target = typeinfos.find(type);
  if (target == typeinfos.end() || target->second.is_polymorphic ||
      ContainsVirtualBase(type))
    return {};
  const auto& typeinfo = target->second;
  for (const auto& [basetype, baseinfo] : typeinfo.baseinfos) {
    assert(
        !baseinfo.IsPolymorphic() &&
        !baseinfo
             .IsVirtual());  // type isn't polymorphic => bases aren't polymorphic
    if (!IsDestructible(basetype))
      return {};
  }
  for (const auto& [fieldname, fieldinfo] : typeinfo.fieldinfos) {
    if (fieldinfo.fieldptr.GetFieldFlag() == FieldFlag::Unowned)
      continue;

    if (!type.IsReference() && !IsDestructible(fieldinfo.fieldptr.GetType()))
      return {};
  }
  const auto& t = target->first;
  return AddMethod(
      type, NameIDRegistry::Meta::dtor,
      MethodInfo{
          {[t](void* obj, void*, ArgsView) {
             const auto& typeinfo = Mngr.typeinfos.at(t);

             for (const auto& [fieldname, fieldinfo] : typeinfo.fieldinfos) {
               if (fieldinfo.fieldptr.GetFieldFlag() == FieldFlag::Unowned)
                 continue;
               Type ftype = fieldinfo.fieldptr.GetType();
               if (ftype.IsReference())
                 continue;
               Mngr.Destruct(fieldinfo.fieldptr.Var(obj));
             }

             for (const auto& [basetype, baseinfo] : typeinfo.baseinfos) {
               void* baseptr = baseinfo.StaticCast_DerivedToBase(obj);
               Mngr.Destruct(ObjectView{basetype, baseptr});
             }
           },
           MethodFlag::Const}});
}

Type ReflMngr::AddBase(Type derived, Type base, BaseInfo baseinfo) {
  auto* typeinfo = GetTypeInfo(derived);
  if (!typeinfo)
    return {};
  auto btarget = typeinfo->baseinfos.find(base.RemoveCVRef());
  if (btarget != typeinfo->baseinfos.end())
    return {};
  Type new_base_type = {tregistry.Register(base.GetID(), base.GetName()),
                        base.GetID()};
  typeinfo->baseinfos.emplace_hint(btarget, new_base_type, std::move(baseinfo));
  return new_base_type;
}

bool ReflMngr::AddTypeAttr(Type type, Attr attr) {
  auto* typeinfo = GetTypeInfo(type);
  if (!typeinfo)
    return false;
  auto& attrs = typeinfo->attrs;
  auto atarget = attrs.find(attr);
  if (atarget != attrs.end())
    return false;
  attrs.emplace_hint(atarget, std::move(attr));
  return true;
}

bool ReflMngr::AddFieldAttr(Type type, Name name, Attr attr) {
  auto* typeinfo = GetTypeInfo(type);
  if (!typeinfo)
    return false;
  auto ftarget = typeinfo->fieldinfos.find(name);
  if (ftarget == typeinfo->fieldinfos.end())
    return false;
  auto& attrs = ftarget->second.attrs;
  auto atarget = attrs.find(attr);
  if (atarget != attrs.end())
    return false;
  attrs.emplace_hint(atarget, std::move(attr));
  return true;
}

bool ReflMngr::AddMethodAttr(Type type, Name name, Attr attr) {
  auto* typeinfo = GetTypeInfo(type);
  if (!typeinfo)
    return false;
  auto mtarget = typeinfo->methodinfos.find(name);
  if (mtarget == typeinfo->methodinfos.end())
    return false;
  auto& attrs = mtarget->second.attrs;
  auto atarget = attrs.find(attr);
  if (atarget != attrs.end())
    return false;
  attrs.emplace_hint(atarget, std::move(attr));
  return true;
}

SharedObject ReflMngr::MMakeShared(Type type, std::pmr::memory_resource* rsrc,
                                   ArgsView args) const {
  if (!IsDestructible(type))
    return {};

  ObjectView obj = MNew(type, rsrc, args);

  if (!obj.GetType().Valid())
    return {};

  return {obj, [rsrc, type](void* ptr) {
            Mngr.MDelete({type, ptr}, rsrc);
          }};
}

ObjectView ReflMngr::StaticCast_DerivedToBase(ObjectView obj, Type type) const {
  if (obj.GetPtr() == nullptr)
    return {type, nullptr};

  const CVRefMode cvref_mode = obj.GetType().GetCVRefMode();
  assert(!CVRefMode_IsVolatile(cvref_mode));
  switch (cvref_mode) {
    case CVRefMode::Left:
      return details::StaticCast_DerivedToBase(obj.RemoveLValueReference(),
                                               type)
          .AddLValueReference();
    case CVRefMode::Right:
      return details::StaticCast_DerivedToBase(obj.RemoveRValueReference(),
                                               type)
          .AddRValueReference();
    case CVRefMode::Const:
      return details::StaticCast_DerivedToBase(obj.RemoveConst(), type)
          .AddConst();
    case CVRefMode::ConstLeft:
      return details::StaticCast_DerivedToBase(obj.RemoveConstReference(), type)
          .AddConstLValueReference();
    case CVRefMode::ConstRight:
      return details::StaticCast_DerivedToBase(obj.RemoveConstReference(), type)
          .AddConstRValueReference();
    default:
      return details::StaticCast_DerivedToBase(obj, type);
  }
}

ObjectView ReflMngr::StaticCast_BaseToDerived(ObjectView obj, Type type) const {
  if (obj.GetPtr() == nullptr)
    return {type, nullptr};

  const CVRefMode cvref_mode = obj.GetType().GetCVRefMode();
  assert(!CVRefMode_IsVolatile(cvref_mode));
  switch (cvref_mode) {
    case CVRefMode::Left:
      return details::StaticCast_BaseToDerived(obj.RemoveLValueReference(),
                                               type)
          .AddLValueReference();
    case CVRefMode::Right:
      return details::StaticCast_BaseToDerived(obj.RemoveRValueReference(),
                                               type)
          .AddRValueReference();
    case CVRefMode::Const:
      return details::StaticCast_BaseToDerived(obj.RemoveConst(), type)
          .AddConst();
    case CVRefMode::ConstLeft:
      return details::StaticCast_BaseToDerived(obj.RemoveConstReference(), type)
          .AddConstLValueReference();
    case CVRefMode::ConstRight:
      return details::StaticCast_BaseToDerived(obj.RemoveConstReference(), type)
          .AddConstRValueReference();
    default:
      return details::StaticCast_BaseToDerived(obj, type);
  }
}

ObjectView ReflMngr::DynamicCast_BaseToDerived(ObjectView obj,
                                               Type type) const {
  if (obj.GetPtr() == nullptr)
    return {type, nullptr};

  const CVRefMode cvref_mode = obj.GetType().GetCVRefMode();
  assert(!CVRefMode_IsVolatile(cvref_mode));
  switch (cvref_mode) {
    case CVRefMode::Left:
      return details::DynamicCast_BaseToDerived(obj.RemoveLValueReference(),
                                                type)
          .AddLValueReference();
    case CVRefMode::Right:
      return details::DynamicCast_BaseToDerived(obj.RemoveRValueReference(),
                                                type)
          .AddRValueReference();
    case CVRefMode::Const:
      return details::DynamicCast_BaseToDerived(obj.RemoveConst(), type)
          .AddConst();
    case CVRefMode::ConstLeft:
      return details::DynamicCast_BaseToDerived(obj.RemoveConstReference(),
                                                type)
          .AddConstLValueReference();
    case CVRefMode::ConstRight:
      return details::DynamicCast_BaseToDerived(obj.RemoveConstReference(),
                                                type)
          .AddConstRValueReference();
    default:
      return details::DynamicCast_BaseToDerived(obj, type);
  }
}

ObjectView ReflMngr::StaticCast(ObjectView obj, Type type) const {
  auto ptr_d2b = StaticCast_DerivedToBase(obj, type);
  if (ptr_d2b.GetType())
    return ptr_d2b;

  auto ptr_b2d = StaticCast_BaseToDerived(obj, type);
  if (ptr_b2d.GetType())
    return ptr_b2d;

  return {};
}

ObjectView ReflMngr::DynamicCast(ObjectView obj, Type type) const {
  auto ptr_b2d = DynamicCast_BaseToDerived(obj, type);
  if (ptr_b2d.GetType())
    return ptr_b2d;

  auto ptr_d2b = StaticCast_DerivedToBase(obj, type);
  if (ptr_d2b.GetType())
    return ptr_d2b;

  return {};
}

ObjectView ReflMngr::Var(ObjectView obj, Name field_name,
                         FieldFlag flag) const {
  if (!obj.GetPtr())
    flag = enum_within(flag, FieldFlag::Unowned);

  const CVRefMode cvref_mode = obj.GetType().GetCVRefMode();
  assert(!CVRefMode_IsVolatile(cvref_mode));
  switch (cvref_mode) {
    case CVRefMode::Left:
      return details::Var(obj.RemoveLValueReference(), field_name, flag)
          .AddLValueReference();
    case CVRefMode::Right:
      return details::Var(obj.RemoveRValueReference(), field_name, flag)
          .AddRValueReference();
    case CVRefMode::Const:
      return details::Var(obj.RemoveConst(), field_name, flag).AddConst();
    case CVRefMode::ConstLeft:
      return details::Var(obj.RemoveConstReference(), field_name, flag)
          .AddConstLValueReference();
    case CVRefMode::ConstRight:
      return details::Var(obj.RemoveConstReference(), field_name, flag)
          .AddConstRValueReference();
    default:
      return details::Var(obj, field_name, flag);
  }
}

ObjectView ReflMngr::Var(ObjectView obj, Type base, Name field_name,
                         FieldFlag flag) const {
  auto base_obj = StaticCast_DerivedToBase(obj, base);
  if (!base_obj.GetType())
    return {};
  return Var(base_obj, field_name);
}

bool ReflMngr::IsCompatible(std::span<const Type> paramTypes,
                            std::span<const Type> argTypes) const {
  if (paramTypes.size() != argTypes.size())
    return false;

  for (size_t i = 0; i < paramTypes.size(); i++) {
    if (paramTypes[i] == argTypes[i] || paramTypes[i].Is<ObjectView>())
      continue;

    const auto& lhs = paramTypes[i];
    const auto& rhs = argTypes[i];

    if (lhs.IsLValueReference()) {  // &{T} | &{const{T}}
      const auto unref_lhs = lhs.Name_RemoveLValueReference();  // T | const{T}
      if (type_name_is_const(unref_lhs)) {                      // &{const{T}}
        if (unref_lhs == rhs.Name_RemoveRValueReference())
          continue;  // &{const{T}} <- &&{const{T}} / const{T}

        const auto raw_lhs = type_name_remove_const(unref_lhs);  // T

        if (rhs.Is(raw_lhs) || raw_lhs == rhs.Name_RemoveReference())
          continue;  // &{const{T}} <- T | &{T} | &&{T}

        if (details::IsRefConstructible(raw_lhs,
                                        std::span<const Type>{&rhs, 1}) &&
            IsDestructible(raw_lhs))
          continue;  // &{const{T}} <- T{arg}
      }
    } else if (lhs.IsRValueReference()) {  // &&{T} | &&{const{T}}
      const auto unref_lhs = lhs.RemoveRValueReference();  // T | const{T}

      if (type_name_is_const(unref_lhs)) {                       // &&{const{T}}
        const auto raw_lhs = type_name_remove_const(unref_lhs);  // T

        if (raw_lhs == type_name_remove_const(rhs))
          continue;  // &&{const{T}} <- T / const{T}

        if (raw_lhs == rhs.Name_RemoveRValueReference())  // &&{const{T}}
          continue;  // &&{const{T}} <- &&{T}

        if (details::IsRefConstructible(raw_lhs,
                                        std::span<const Type>{&rhs, 1}))
          continue;  // &&{const{T}} <- T{arg}
      } else {
        if (rhs.Is(unref_lhs))
          continue;  // &&{T} <- T

        if (details::IsRefConstructible(unref_lhs,
                                        std::span<const Type>{&rhs, 1}) &&
            IsDestructible(unref_lhs))
          continue;  // &&{T} <- T{arg}
      }
    } else {  // T
      if (lhs.Is(rhs.Name_RemoveRValueReference()))
        continue;  // T <- &&{T}

      if (details::IsRefConstructible(lhs, std::span<const Type>{&rhs, 1}) &&
          IsDestructible(lhs))
        continue;  // T <- T{arg}
    }

    if (is_pointer_array_compatible(lhs, rhs))
      continue;

    return false;
  }

  return true;
}

Type ReflMngr::IsInvocable(Type type, Name method_name,
                           std::span<const Type> argTypes,
                           MethodFlag flag) const {
  Type rawtype;
  MethodFlag newflag;
  const CVRefMode cvref_mode = type.GetCVRefMode();
  assert(!CVRefMode_IsVolatile(cvref_mode));
  switch (cvref_mode) {
    case CVRefMode::Left:
      [[fallthrough]];
    case CVRefMode::Right:
      rawtype = type.RemoveReference();
      newflag = flag;
      break;
    case CVRefMode::Const:
      [[fallthrough]];
    case CVRefMode::ConstLeft:
      [[fallthrough]];
    case CVRefMode::ConstRight:
      rawtype = type.RemoveCVRef();
      newflag = enum_remove(flag, MethodFlag::Variable);
      break;
    default:
      rawtype = type;
      newflag = flag;
      break;
  }

  if (auto priority_rst =
          details::IsInvocable(true, rawtype, method_name, argTypes, newflag))
    return priority_rst;

  return details::IsInvocable(false, rawtype, method_name, argTypes, newflag);
}

Type ReflMngr::BInvoke(ObjectView obj, Name method_name, void* result_buffer,
                       ArgsView args, MethodFlag flag,
                       std::pmr::memory_resource* temp_args_rsrc) const {
  assert(temp_args_rsrc);

  ObjectView rawObj;
  const CVRefMode cvref_mode = obj.GetType().GetCVRefMode();
  assert(!CVRefMode_IsVolatile(cvref_mode));
  switch (cvref_mode) {
    case CVRefMode::Left:
      [[fallthrough]];
    case CVRefMode::Right:
      rawObj = obj.RemoveReference();
      break;
    case CVRefMode::Const:
      rawObj = obj.RemoveConst();
      flag = enum_remove(flag, MethodFlag::Variable);
      break;
    case CVRefMode::ConstLeft:
      [[fallthrough]];
    case CVRefMode::ConstRight:
      rawObj = obj.RemoveConstReference();
      flag = enum_remove(flag, MethodFlag::Variable);
      break;
    default:
      rawObj = obj;
      break;
  }

  if (!obj.GetPtr())
    flag = enum_within(flag, MethodFlag::Static);

  if (auto priority_rst = details::BInvoke(
          true, temp_args_rsrc, rawObj, method_name, result_buffer, args, flag))
    return priority_rst;

  return details::BInvoke(false, temp_args_rsrc, rawObj, method_name,
                          result_buffer, args, flag);
}

SharedObject ReflMngr::MInvoke(
    ObjectView obj, Name method_name, std::pmr::memory_resource* rst_rsrc,
    ArgsView args, MethodFlag flag,
    std::pmr::memory_resource* temp_args_rsrc) const {
  assert(rst_rsrc);
  assert(temp_args_rsrc);

  ObjectView rawObj;
  const CVRefMode cvref_mode = obj.GetType().GetCVRefMode();
  assert(!CVRefMode_IsVolatile(cvref_mode));
  switch (cvref_mode) {
    case CVRefMode::Left:
      [[fallthrough]];
    case CVRefMode::Right:
      rawObj = obj.RemoveReference();
      break;
    case CVRefMode::Const:
      rawObj = obj.RemoveConst();
      flag = enum_remove(flag, MethodFlag::Variable);
      break;
    case CVRefMode::ConstLeft:
      [[fallthrough]];
    case CVRefMode::ConstRight:
      rawObj = obj.RemoveConstReference();
      flag = enum_remove(flag, MethodFlag::Variable);
      break;
    default:
      rawObj = obj;
      break;
  }

  if (!obj.GetPtr())
    flag = enum_within(flag, MethodFlag::Static);

  if (auto priority_rst = details::MInvoke(true, temp_args_rsrc, rawObj,
                                           method_name, rst_rsrc, args, flag);
      priority_rst.GetType().Valid())
    return priority_rst;

  return details::MInvoke(false, temp_args_rsrc, rawObj, method_name, rst_rsrc,
                          args, flag);
}

ObjectView ReflMngr::MNew(Type type, std::pmr::memory_resource* rsrc,
                          ArgsView args) const {
  assert(rsrc);

  if (!IsConstructible(type, args.Types()))
    return {};

  const auto& typeinfo = typeinfos.at(type);

  void* buffer = rsrc->allocate(std::max<std::size_t>(1, typeinfo.size),
                                typeinfo.alignment);

  if (!buffer)
    return {};

  ObjectView obj{type, buffer};
  bool success = Construct(obj, args);
  assert(success);

  return obj;
}

bool ReflMngr::MDelete(ObjectView obj, std::pmr::memory_resource* rsrc) const {
  assert(rsrc);

  Destruct(obj);

  const auto& typeinfo = typeinfos.at(obj.GetType());

  rsrc->deallocate(obj.GetPtr(), std::max<std::size_t>(1, typeinfo.size),
                   typeinfo.alignment);

  return true;
}

ObjectView ReflMngr::New(Type type, ArgsView args) const {
  return MNew(type, &object_resource, args);
}

bool ReflMngr::Delete(ObjectView obj) const {
  return MDelete(obj, &object_resource);
}

SharedObject ReflMngr::MakeShared(Type type, ArgsView args) const {
  return MMakeShared(type, &object_resource, args);
}

bool ReflMngr::IsConstructible(Type type,
                               std::span<const Type> argTypes) const {
  auto target = typeinfos.find(type);
  if (target == typeinfos.end())
    return false;
  const auto& typeinfo = target->second;
  if (argTypes.empty() && typeinfo.is_trivial)
    return true;
  auto [begin_iter, end_iter] =
      typeinfo.methodinfos.equal_range(NameIDRegistry::Meta::ctor);
  for (auto iter = begin_iter; iter != end_iter; ++iter) {
    if (IsCompatible(iter->second.methodptr.GetParamList(), argTypes))
      return true;
  }
  return false;
}

bool ReflMngr::IsCopyConstructible(Type type) const {
  const Type clref_type = tregistry.RegisterAddConstLValueReference(type);
  return details::IsRefConstructible(type,
                                     std::span<const Type>{&clref_type, 1});
}

bool ReflMngr::IsMoveConstructible(Type type) const {
  const Type rref_type = tregistry.RegisterAddRValueReference(type);
  return details::IsRefConstructible(type,
                                     std::span<const Type>{&rref_type, 1});
}

bool ReflMngr::IsDestructible(Type type) const {
  assert(type.GetCVRefMode() == CVRefMode::None);

  auto target = typeinfos.find(type);
  if (target == typeinfos.end())
    return false;
  const auto& typeinfo = target->second;
  if (typeinfo.is_trivial)
    return true;
  auto [begin_iter, end_iter] =
      typeinfo.methodinfos.equal_range(NameIDRegistry::Meta::dtor);
  if (begin_iter == end_iter)
    return true;
  for (auto iter = begin_iter; iter != end_iter; ++iter) {
    if (iter->second.methodptr.GetMethodFlag() == MethodFlag::Const &&
        IsCompatible(iter->second.methodptr.GetParamList(), {}))
      return true;
  }
  return false;
}

bool ReflMngr::Construct(ObjectView obj, ArgsView args) const {
  auto target = typeinfos.find(obj.GetType());
  if (target == typeinfos.end())
    return false;
  const auto& typeinfo = target->second;
  if (args.Types().empty() && typeinfo.is_trivial)
    return true;  // trivial ctor
  auto [begin_iter, end_iter] =
      typeinfo.methodinfos.equal_range(NameIDRegistry::Meta::ctor);
  for (auto iter = begin_iter; iter != end_iter; ++iter) {
    if (iter->second.methodptr.GetMethodFlag() == MethodFlag::Variable) {
      details::NewArgsGuard guard{false, &temporary_resource,
                                  iter->second.methodptr.GetParamList(), args};
      if (!guard.IsCompatible())
        continue;
      iter->second.methodptr.Invoke(obj.GetPtr(), nullptr, guard.GetArgsView());
      return true;
    }
  }
  return false;
}

bool ReflMngr::Destruct(ObjectView obj) const {
  auto target = typeinfos.find(obj.GetType());
  if (target == typeinfos.end())
    return false;
  const auto& typeinfo = target->second;
  if (typeinfo.is_trivial)
    return true;  // trivial ctor
  auto [begin_iter, end_iter] =
      typeinfo.methodinfos.equal_range(NameIDRegistry::Meta::dtor);
  for (auto iter = begin_iter; iter != end_iter; ++iter) {
    if (iter->second.methodptr.GetMethodFlag() == MethodFlag::Const &&
        IsCompatible(iter->second.methodptr.GetParamList(), {})) {
      iter->second.methodptr.Invoke(obj.GetPtr(), nullptr, {});
      return true;
    }
  }
  return false;
}

void ReflMngr::ForEachTypeInfo(
    Type type, const std::function<bool(InfoTypePair)>& func) const {
  std::set<TypeID> visitedVBs;
  details::ForEachTypeInfo(type.RemoveCVRef(), func, visitedVBs);
}

void ReflMngr::ForEachField(
    Type type, const std::function<bool(InfoTypePair, InfoFieldPair)>& func,
    FieldFlag flag) const {
  ForEachTypeInfo(type, [flag, &func](InfoTypePair type) {
    for (auto& [field, fieldInfo] : type.info->fieldinfos) {
      if (!enum_contain(flag, fieldInfo.fieldptr.GetFieldFlag()))
        continue;

      if (!func(type, {field, &fieldInfo}))
        return false;
    }
    return true;
  });
}

void ReflMngr::ForEachMethod(
    Type type, const std::function<bool(InfoTypePair, InfoMethodPair)>& func,
    MethodFlag flag) const {
  ForEachTypeInfo(type, [flag, &func](InfoTypePair type) {
    for (auto& [method_name, methodInfo] : type.info->methodinfos) {
      if (!enum_contain(flag, methodInfo.methodptr.GetMethodFlag()))
        continue;

      if (!func(type, {method_name, &methodInfo}))
        return false;
    }
    return true;
  });
}

void ReflMngr::ForEachVar(
    ObjectView obj,
    const std::function<bool(InfoTypePair, InfoFieldPair, ObjectView)>& func,
    FieldFlag flag) const {
  if (!obj.GetPtr())
    flag = enum_within(flag, FieldFlag::Unowned);

  std::set<TypeID> visitedVBs;
  const CVRefMode cvref_mode = obj.GetType().GetCVRefMode();
  assert(!CVRefMode_IsVolatile(cvref_mode));
  switch (cvref_mode) {
    case CVRefMode::Left:
      details::ForEachVar(
          obj.RemoveLValueReference(),
          [&func](InfoTypePair t, InfoFieldPair f, ObjectView o) {
            return func(t, f, o.AddLValueReference());
          },
          flag, visitedVBs);
      break;
    case CVRefMode::Right:
      details::ForEachVar(
          obj.RemoveRValueReference(),
          [&func](InfoTypePair t, InfoFieldPair f, ObjectView o) {
            return func(t, f, o.AddRValueReference());
          },
          flag, visitedVBs);
      break;
    case CVRefMode::Const:
      details::ForEachVar(
          obj.RemoveConst(),
          [&func](InfoTypePair t, InfoFieldPair f, ObjectView o) {
            return func(t, f, o.AddConst());
          },
          flag, visitedVBs);
      break;
    case CVRefMode::ConstLeft:
      details::ForEachVar(
          obj.RemoveConstReference(),
          [&func](InfoTypePair t, InfoFieldPair f, ObjectView o) {
            return func(t, f, o.AddConstLValueReference());
          },
          flag, visitedVBs);
      break;
    case CVRefMode::ConstRight:
      details::ForEachVar(
          obj.RemoveConstReference(),
          [&func](InfoTypePair t, InfoFieldPair f, ObjectView o) {
            return func(t, f, o.AddConstRValueReference());
          },
          flag, visitedVBs);
      break;
    default:
      details::ForEachVar(obj, func, flag, visitedVBs);
      break;
  }
}

std::vector<InfoTypePair> ReflMngr::GetTypes(Type type) {
  std::vector<InfoTypePair> rst;
  ForEachTypeInfo(type, [&rst](InfoTypePair type) {
    rst.push_back(type);
    return true;
  });
  return rst;
}

std::vector<InfoTypeFieldPair> ReflMngr::GetTypeFields(Type type,
                                                       FieldFlag flag) {
  std::vector<InfoTypeFieldPair> rst;
  ForEachField(
      type,
      [&rst](InfoTypePair type, InfoFieldPair field) {
        rst.emplace_back(InfoTypeFieldPair{type, field});
        return true;
      },
      flag);
  return rst;
}

std::vector<InfoFieldPair> ReflMngr::GetFields(Type type, FieldFlag flag) {
  std::vector<InfoFieldPair> rst;
  ForEachField(
      type,
      [&rst](InfoTypePair type, InfoFieldPair field) {
        rst.push_back(field);
        return true;
      },
      flag);
  return rst;
}

std::vector<InfoTypeMethodPair> ReflMngr::GetTypeMethods(Type type,
                                                         MethodFlag flag) {
  std::vector<InfoTypeMethodPair> rst;
  ForEachMethod(
      type,
      [&rst](InfoTypePair type, InfoMethodPair field) {
        rst.emplace_back(InfoTypeMethodPair{type, field});
        return true;
      },
      flag);
  return rst;
}

std::vector<InfoMethodPair> ReflMngr::GetMethods(Type type, MethodFlag flag) {
  std::vector<InfoMethodPair> rst;
  ForEachMethod(
      type,
      [&rst](InfoTypePair type, InfoMethodPair field) {
        rst.push_back(field);
        return true;
      },
      flag);
  return rst;
}

std::vector<std::tuple<InfoTypePair, InfoFieldPair, ObjectView>>
ReflMngr::GetTypeFieldVars(ObjectView obj, FieldFlag flag) {
  std::vector<std::tuple<InfoTypePair, InfoFieldPair, ObjectView>> rst;
  ForEachVar(
      obj,
      [&rst](InfoTypePair type, InfoFieldPair field, ObjectView var) {
        rst.emplace_back(std::tuple{type, field, var});
        return true;
      },
      flag);
  return rst;
}

std::vector<ObjectView> ReflMngr::GetVars(ObjectView obj, FieldFlag flag) {
  std::vector<ObjectView> rst;
  ForEachVar(
      obj,
      [&rst](InfoTypePair type, InfoFieldPair field, ObjectView var) {
        rst.push_back(var);
        return true;
      },
      flag);
  return rst;
}

InfoTypePair ReflMngr::FindType(
    Type type, const std::function<bool(InfoTypePair)>& func) const {
  InfoTypePair rst;
  ForEachTypeInfo(type, [&rst, func](InfoTypePair type) {
    if (!func(type))
      return true;

    rst = type;
    return false;  // stop
  });
  return rst;
}

InfoFieldPair ReflMngr::FindField(
    Type type, const std::function<bool(InfoFieldPair)>& func,
    FieldFlag flag) const {
  InfoFieldPair rst;
  ForEachField(
      type,
      [&rst, func](InfoTypePair type, InfoFieldPair field) {
        if (!func(field))
          return true;

        rst = field;
        return false;  // stop
      },
      flag);
  return rst;
}

InfoMethodPair ReflMngr::FindMethod(
    Type type, const std::function<bool(InfoMethodPair)>& func,
    MethodFlag flag) const {
  InfoMethodPair rst;
  ForEachMethod(
      type,
      [&rst, func](InfoTypePair type, InfoMethodPair method) {
        if (!func(method))
          return true;

        rst = method;
        return false;  // stop
      },
      flag);
  return rst;
}

ObjectView ReflMngr::FindVar(ObjectView obj,
                             const std::function<bool(ObjectView)>& func,
                             FieldFlag flag) const {
  ObjectView rst;
  ForEachVar(
      obj,
      [&rst, func](InfoTypePair type, InfoFieldPair field, ObjectView obj) {
        if (!func(obj))
          return true;

        rst = obj;
        return false;  // stop
      },
      flag);
  return rst;
}

bool ReflMngr::ContainsBase(Type type, Type base) const {
  auto* info = GetTypeInfo(type);
  if (info->baseinfos.contains(base.RemoveCVRef()))
    return true;

  for (const auto& [basetype, baseinfo] : info->baseinfos) {
    bool found = ContainsBase(basetype, base);
    if (found)
      return true;
  }
  return false;
}

bool ReflMngr::ContainsField(Type type, Name field_name, FieldFlag flag) const {
  const CVRefMode cvref_mode = type.GetCVRefMode();
  assert(!CVRefMode_IsVolatile(cvref_mode));
  switch (cvref_mode) {
    case CVRefMode::Left:
      return details::ContainsField(type.RemoveLValueReference(), field_name,
                                    flag);
    case CVRefMode::Right:
      return details::ContainsField(type.RemoveRValueReference(), field_name,
                                    flag);
    case CVRefMode::Const:
      return details::ContainsField(type.RemoveConst(), field_name, flag);
    case CVRefMode::ConstLeft:
    case CVRefMode::ConstRight:
      return details::ContainsField(type.RemoveCVRef(), field_name, flag);
    default:
      return details::ContainsField(type, field_name, flag);
  }
}

bool ReflMngr::ContainsMethod(Type type, Name method_name,
                              MethodFlag flag) const {
  const CVRefMode cvref_mode = type.GetCVRefMode();
  assert(!CVRefMode_IsVolatile(cvref_mode));
  switch (cvref_mode) {
    case CVRefMode::Left:
      return details::ContainsMethod(type.RemoveLValueReference(), method_name,
                                     flag);
    case CVRefMode::Right:
      return details::ContainsMethod(type.RemoveRValueReference(), method_name,
                                     flag);
    case CVRefMode::Const:
      return details::ContainsMethod(type.RemoveConst(), method_name, flag);
    case CVRefMode::ConstLeft:
    case CVRefMode::ConstRight:
      return details::ContainsMethod(type.RemoveCVRef(), method_name, flag);
    default:
      return details::ContainsMethod(type, method_name, flag);
  }
}
