//
// Created by Admin on 1/04/2025.
//

#pragma once

#include "Object.h"

#include <variant>
#include <vector>

namespace My::MyDRefl {
using ParamList = std::vector<TypeID>;

class ArgsView {
 public:
  ArgsView(ArgsBuffer buffer, const ParamList& paramList)
      : buffer{buffer}, paramList{paramList} {}

  ArgsBuffer GetBuffer() const noexcept { return buffer; }

  const ParamList& GetParamList() const noexcept { return paramList; }

  // pointer to
  // - non-const argument
  // - referenced object : call Dereference / DereferenceAsConst later
  ObjectPtr At(size_t idx) const noexcept {
    assert(idx < paramList.size());
    return {paramList[idx], buffer[idx]};
  }

 private:
  ArgsBuffer buffer;
  const ParamList& paramList;
};

class MethodPtr {
 public:
  using MemberVariableFunction = Destructor(void*, void*, ArgsView);
  using MemberConstFunction = Destructor(const void*, void*, ArgsView);
  using StaticFunction = Destructor(void*, ArgsView);

  MethodPtr(std::function<MemberVariableFunction> func,
            ResultDesc resultDesc = {}, ParamList paramList = {})
      : func{(assert(func), std::move(func))},
        resultDesc{std::move(resultDesc)},
        paramList{std::move(paramList)} {}

  MethodPtr(std::function<MemberConstFunction> func, ResultDesc resultDesc = {},
            ParamList paramList = {})
      : func{(assert(func), std::move(func))},
        resultDesc{std::move(resultDesc)},
        paramList{std::move(paramList)} {}

  MethodPtr(std::function<StaticFunction> func, ResultDesc resultDesc = {},
            ParamList paramList = {})
      : func{(assert(func), std::move(func))},
        resultDesc{std::move(resultDesc)},
        paramList{std::move(paramList)} {}

  MethodPtr(MemberVariableFunction* func, ResultDesc resultDesc = {},
            ParamList paramList = {})
      : MethodPtr{std::function<MemberVariableFunction>{func},
                  std::move(resultDesc), std::move(paramList)} {
    assert(func);
  }

  MethodPtr(MemberConstFunction* func, ResultDesc resultDesc = {},
            ParamList paramList = {})
      : MethodPtr{std::function<MemberConstFunction>{func},
                  std::move(resultDesc), std::move(paramList)} {
    assert(func);
  }

  MethodPtr(StaticFunction* func, ResultDesc resultDesc = {},
            ParamList paramList = {})
      : MethodPtr{std::function<StaticFunction>{func}, std::move(resultDesc),
                  std::move(paramList)} {
    assert(func);
  }

  bool IsMemberVariable() const noexcept { return func.index() == 0; }

  bool IsMemberConst() const noexcept { return func.index() == 1; }

  bool IsStatic() const noexcept { return func.index() == 2; }

  const ParamList& GetParamList() const noexcept { return paramList; }

  const ResultDesc& GetResultDesc() const noexcept { return resultDesc; }

  bool IsDistinguishableWith(const MethodPtr& rhs) const noexcept {
    return func.index() != rhs.func.index() || paramList != rhs.paramList;
  }

  Destructor Invoke(void* obj, void* result_buffer,
                    ArgsBuffer args_buffer) const;
  Destructor Invoke(const void* obj, void* result_buffer,
                    ArgsBuffer args_buffer) const;
  Destructor Invoke(void* result_buffer, ArgsBuffer args_buffer) const;

  Destructor Invoke_Static(void* result_buffer, ArgsBuffer args_buffer) const {
    assert(IsStatic());
    return std::get<std::function<StaticFunction>>(func)(
        result_buffer, {args_buffer, paramList});
  };

 private:
  std::variant<std::function<MemberVariableFunction>,
               std::function<MemberConstFunction>,
               std::function<StaticFunction>>
      func;
  ResultDesc resultDesc;
  ParamList paramList;
};
}  // namespace My::MyDRefl
