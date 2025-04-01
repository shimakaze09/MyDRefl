//
// Created by Admin on 11/03/2025.
//

#pragma once

#include "BaseInfo.h"
#include "FieldInfo.h"
#include "MethodInfo.h"

namespace My::MyDRefl {
struct TypeInfo {
  //
  // Data
  /////////

  std::unordered_map<NameID, FieldInfo> fieldinfos;
  std::unordered_multimap<NameID, MethodInfo> methodinfos;
  std::unordered_map<TypeID, BaseInfo> baseinfos;
  std::unordered_map<TypeID, std::any> attrs;

  //
  // Field
  //////////

  // without bases, read/write field, static non-const
  ObjectPtr RWField(NameID fieldID) const noexcept;
  // without bases, read/write field, static non-const + static const
  ConstObjectPtr RField(NameID fieldID) const noexcept;
  // without bases, read/write field, non-const + static non-const
  ObjectPtr RWField(void* obj, NameID fieldID) const noexcept;
  // without bases, read field, non-const + static non-const + static non-const + static const
  ConstObjectPtr RField(const void* obj, NameID fieldID) const noexcept;

  //
  // Invoke
  ///////////

  // without bases, static
  bool IsStaticInvocable(NameID methodID,
                         Span<TypeID> argTypeIDs) const noexcept;
  // without bases, const + static
  bool IsConstInvocable(NameID methodID,
                        Span<TypeID> argTypeIDs) const noexcept;
  // without bases, non-const + const + static
  bool IsInvocable(NameID methodID, Span<TypeID> argTypeIDs) const noexcept;

  // without bases, static
  InvokeResult Invoke(NameID methodID, Span<TypeID> argTypeIDs,
                      void* args_buffer, void* result_buffer) const;
  // without bases, const + static
  InvokeResult Invoke(const void* obj, NameID methodID, Span<TypeID> argTypeIDs,
                      void* args_buffer, void* result_buffer) const;
  // without bases, non-const + const + static, non-const && static > const
  InvokeResult Invoke(void* obj, NameID methodID, Span<TypeID> argTypeIDs,
                      void* args_buffer, void* result_buffer) const;
};
}  // namespace My::MyDRefl
