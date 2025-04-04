//
// Created by Admin on 3/04/2025.
//

#include <MyDRefl/Object.h>

#include <MyDRefl/ReflMngr.h>

using namespace My;
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

std::pmr::vector<TypeID> ObjectPtrBase::GetTypeIDs() {
  return ReflMngr::Instance().GetTypeIDs(ID);
}

std::pmr::vector<TypeRef> ObjectPtrBase::GetTypes() {
  return ReflMngr::Instance().GetTypes(ID);
}

std::pmr::vector<TypeFieldRef> ObjectPtrBase::GetTypeFields() {
  return ReflMngr::Instance().GetTypeFields(ID);
}

std::pmr::vector<FieldRef> ObjectPtrBase::GetFields() {
  return ReflMngr::Instance().GetFields(ID);
}

std::pmr::vector<TypeMethodRef> ObjectPtrBase::GetTypeMethods() {
  return ReflMngr::Instance().GetTypeMethods(ID);
}

std::pmr::vector<MethodRef> ObjectPtrBase::GetMethods() {
  return ReflMngr::Instance().GetMethods(ID);
}

std::pmr::vector<std::tuple<TypeRef, FieldRef, ConstObjectPtr>>
ObjectPtrBase::GetTypeFieldRVars() {
  return ReflMngr::Instance().GetTypeFieldRVars({ID, ptr});
}

std::pmr::vector<ConstObjectPtr> ObjectPtrBase::GetRVars() {
  return ReflMngr::Instance().GetRVars({ID, ptr});
}

//
// ConstObjectPtr
///////////////////

ConstObjectPtr::ConstObjectPtr(const SharedConstObject& obj) noexcept
    : ConstObjectPtr{obj.GetID(), obj.GetPtr()} {}

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
                                 Span<const TypeID> argTypeIDs) const noexcept {
  return ReflMngr::Instance().IsConstInvocable(ID, methodID, argTypeIDs);
}

InvokeResult ConstObjectPtr::Invoke(StrID methodID, void* result_buffer,
                                    Span<const TypeID> argTypeIDs,
                                    void* args_buffer) const {
  return ReflMngr::Instance().Invoke(*this, methodID, result_buffer, argTypeIDs,
                                     args_buffer);
}

SharedObject ConstObjectPtr::MInvoke(
    StrID methodID, Span<const TypeID> argTypeIDs, void* args_buffer,
    MemoryResourceType memory_rsrc_type) const {
  return ReflMngr::Instance().MInvoke(*this, methodID, argTypeIDs, args_buffer,
                                      memory_rsrc_type);
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
                            Span<const TypeID> argTypeIDs) const noexcept {
  return ReflMngr::Instance().IsInvocable(ID, methodID, argTypeIDs);
}

InvokeResult ObjectPtr::Invoke(StrID methodID, void* result_buffer,
                               Span<const TypeID> argTypeIDs,
                               void* args_buffer) const {
  return ReflMngr::Instance().Invoke(*this, methodID, result_buffer, argTypeIDs,
                                     args_buffer);
}

SharedObject ObjectPtr::MInvoke(StrID methodID, Span<const TypeID> argTypeIDs,
                                void* args_buffer,
                                MemoryResourceType memory_rsrc_type) const {
  return ReflMngr::Instance().MInvoke(*this, methodID, argTypeIDs, args_buffer,
                                      memory_rsrc_type);
}

void ObjectPtr::ForEachRWVar(
    const std::function<bool(TypeRef, FieldRef, ObjectPtr)>& func) const {
  return ReflMngr::Instance().ForEachRWVar(*this, func);
}

std::pmr::vector<std::tuple<TypeRef, FieldRef, ObjectPtr>>
ObjectPtr::GetTypeFieldRWVars() {
  return ReflMngr::Instance().GetTypeFieldRWVars(*this);
}

std::pmr::vector<ObjectPtr> ObjectPtr::GetRWVars() {
  return ReflMngr::Instance().GetRWVars(*this);
}
