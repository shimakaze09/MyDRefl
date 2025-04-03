//
// Created by Admin on 3/04/2025.
//

#include <MyDRefl/Object.h>

#include <MyDRefl/ReflMngr.h>

using namespace My::MyDRefl;

//
// ObjectPtrBase
//////////////////

ConstObjectPtr ObjectPtrBase::RVar(StrID fieldID) const noexcept {
  return ReflMngr::Instance().RVar({ID, ptr}, fieldID);
}

ConstObjectPtr ObjectPtrBase::RVar(TypeID baseID,
                                   StrID fieldID) const noexcept {
  return ReflMngr::Instance().RVar({ID, ptr}, baseID, fieldID);
}

// self [r] vars and all bases' [r] vars
void ObjectPtrBase::ForEachRVar(
    const std::function<bool(TypeRef, FieldRef, ConstObjectPtr)>& func) const {
  return ReflMngr::Instance().ForEachRVar({ID, ptr}, func);
}

//
// ConstObjectPtr
///////////////////

ConstObjectPtr ConstObjectPtr::StaticCast_DerivedToBase(
    TypeID typeID) const noexcept {
  return ReflMngr::Instance().StaticCast_DerivedToBase(*this, typeID);
}

ConstObjectPtr ConstObjectPtr::StaticCast_BaseToDerived(
    TypeID typeID) const noexcept {
  return ReflMngr::Instance().StaticCast_BaseToDerived(*this, typeID);
}

ConstObjectPtr ConstObjectPtr::DynamicCast_BaseToDerived(
    TypeID typeID) const noexcept {
  return ReflMngr::Instance().DynamicCast_BaseToDerived(*this, typeID);
}

ConstObjectPtr ConstObjectPtr::StaticCast(TypeID typeID) const noexcept {
  return ReflMngr::Instance().StaticCast(*this, typeID);
}

ConstObjectPtr ConstObjectPtr::DynamicCast(TypeID typeID) const noexcept {
  return ReflMngr::Instance().DynamicCast(*this, typeID);
}

bool ConstObjectPtr::IsInvocable(StrID methodID,
                                 Span<TypeID> argTypeIDs) const noexcept {
  return ReflMngr::Instance().IsConstInvocable(ID, methodID, argTypeIDs);
}

InvokeResult ConstObjectPtr::Invoke(StrID methodID, Span<TypeID> argTypeIDs,
                                    void* args_buffer,
                                    void* result_buffer) const {
  return ReflMngr::Instance().Invoke(*this, methodID, argTypeIDs, args_buffer,
                                     result_buffer);
}

//
// ObjectPtr
//////////////

ObjectPtr ObjectPtr::StaticCast_DerivedToBase(TypeID typeID) const noexcept {
  return ReflMngr::Instance().StaticCast_DerivedToBase(*this, typeID);
}

ObjectPtr ObjectPtr::StaticCast_BaseToDerived(TypeID typeID) const noexcept {
  return ReflMngr::Instance().StaticCast_BaseToDerived(*this, typeID);
}

ObjectPtr ObjectPtr::DynamicCast_BaseToDerived(TypeID typeID) const noexcept {
  return ReflMngr::Instance().DynamicCast_BaseToDerived(*this, typeID);
}

ObjectPtr ObjectPtr::StaticCast(TypeID typeID) const noexcept {
  return ReflMngr::Instance().StaticCast(*this, typeID);
}

ObjectPtr ObjectPtr::DynamicCast(TypeID typeID) const noexcept {
  return ReflMngr::Instance().DynamicCast(*this, typeID);
}

ObjectPtr ObjectPtr::RWVar(StrID fieldID) const noexcept {
  return ReflMngr::Instance().RWVar(*this, fieldID);
}

ObjectPtr ObjectPtr::RWVar(TypeID baseID, StrID fieldID) const noexcept {
  return ReflMngr::Instance().RWVar(*this, baseID, fieldID);
}

bool ObjectPtr::IsInvocable(StrID methodID,
                            Span<TypeID> argTypeIDs) const noexcept {
  return ReflMngr::Instance().IsInvocable(ID, methodID, argTypeIDs);
}

InvokeResult ObjectPtr::Invoke(StrID methodID, Span<TypeID> argTypeIDs,
                               void* args_buffer, void* result_buffer) const {
  return ReflMngr::Instance().Invoke(*this, methodID, argTypeIDs, args_buffer,
                                     result_buffer);
}

void ObjectPtr::ForEachRWVar(
    const std::function<bool(TypeRef, FieldRef, ObjectPtr)>& func) const {
  return ReflMngr::Instance().ForEachRWVar(*this, func);
}
