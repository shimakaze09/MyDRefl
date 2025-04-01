//
// Created by Admin on 1/04/2025.
//

#include <MyDRefl/TypeInfo.h>

#include <MyDRefl/ReflMngr.h>

using namespace My::MyDRefl;

ObjectPtr TypeInfo::RWField(size_t fieldID) const noexcept {
  auto target = fieldinfos.find(fieldID);
  if (target == fieldinfos.end())
    return nullptr;

  if (target->second.fieldptr.GetType() != FieldPtr::Type::STATIC_VARIABLE)
    return nullptr;

  return target->second.fieldptr.Map_StaticVariable();
}

ConstObjectPtr TypeInfo::RField(size_t fieldID) const noexcept {
  auto target = fieldinfos.find(fieldID);
  if (target == fieldinfos.end())
    return nullptr;

  if (!target->second.fieldptr.IsStatic())
    return nullptr;

  return target->second.fieldptr.Map();
}

ObjectPtr TypeInfo::RWField(void* obj, size_t fieldID) const noexcept {
  auto target = fieldinfos.find(fieldID);
  if (target == fieldinfos.end())
    return nullptr;

  if (target->second.fieldptr.IsConst())
    return nullptr;

  return target->second.fieldptr.Map(obj);
}

ConstObjectPtr TypeInfo::RField(const void* obj,
                                size_t fieldID) const noexcept {
  auto target = fieldinfos.find(fieldID);
  if (target == fieldinfos.end())
    return nullptr;

  return target->second.fieldptr.Map(obj);
}

bool TypeInfo::IsStaticInvocable(size_t methodID,
                                 Span<size_t> argTypeIDs) const noexcept {
  auto target = smethodinfos.find(methodID);
  size_t num = smethodinfos.count(methodID);
  for (size_t i = 0; i < num; ++i, ++target) {
    if (target->second.method.paramList.IsConpatibleWith(argTypeIDs))
      return true;
  }
  return false;
}

bool TypeInfo::IsConstInvocable(size_t methodID,
                                Span<size_t> argTypeIDs) const noexcept {
  auto target = cmethodinfos.find(methodID);
  size_t num = cmethodinfos.count(methodID);
  for (size_t i = 0; i < num; ++i, ++target) {
    if (target->second.method.paramList.IsConpatibleWith(argTypeIDs))
      return true;
  }
  return IsStaticInvocable(methodID, argTypeIDs);
}

bool TypeInfo::IsInvocable(size_t methodID,
                           Span<size_t> argTypeIDs) const noexcept {
  auto target = methodinfos.find(methodID);
  size_t num = methodinfos.count(methodID);
  for (size_t i = 0; i < num; ++i, ++target) {
    if (target->second.method.paramList.IsConpatibleWith(argTypeIDs))
      return true;
  }
  return IsConstInvocable(methodID, argTypeIDs);
}

InvokeResult TypeInfo::Invoke(size_t methodID, Span<size_t> argTypeIDs,
                              void* buffer) const {
  auto target = smethodinfos.find(methodID);
  size_t num = smethodinfos.count(methodID);
  for (size_t i = 0; i < num; ++i, ++target) {
    if (target->second.method.paramList.IsConpatibleWith(argTypeIDs))
      return {true, target->second.method.Invoke(buffer)};
  }
  return {false, {}};
}

InvokeResult TypeInfo::Invoke(const void* obj, size_t methodID,
                              Span<size_t> argTypeIDs, void* buffer) const {
  auto target = cmethodinfos.find(methodID);
  size_t num = cmethodinfos.count(methodID);
  for (size_t i = 0; i < num; ++i, ++target) {
    if (target->second.method.paramList.IsConpatibleWith(argTypeIDs))
      return {true, target->second.method.Invoke(obj, buffer)};
  }
  return Invoke(methodID, argTypeIDs, buffer);
}

InvokeResult TypeInfo::Invoke(void* obj, size_t methodID,
                              Span<size_t> argTypeIDs, void* buffer) const {
  auto target = methodinfos.find(methodID);
  size_t num = methodinfos.count(methodID);
  for (size_t i = 0; i < num; ++i, ++target) {
    if (target->second.method.paramList.IsConpatibleWith(argTypeIDs))
      return {true, target->second.method.Invoke(obj, buffer)};
  }
  return Invoke(static_cast<const void*>(obj), methodID, argTypeIDs, buffer);
}
