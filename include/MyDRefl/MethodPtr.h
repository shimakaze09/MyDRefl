#pragma once

#include "Object.h"

#include <variant>
#include <vector>

namespace My::MyDRefl {
using ParamList = std::vector<Type>;

class ArgsView {
 public:
  ArgsView(ArgPtrBuffer buffer, const ParamList& paramList)
      : buffer{buffer}, paramList{paramList} {}

  ArgPtrBuffer GetBuffer() const noexcept { return buffer; }

  const ParamList& GetParamList() const noexcept { return paramList; }

  ObjectView At(size_t idx) const noexcept {
    assert(idx < paramList.size());
    return {paramList[idx], buffer[idx]};
  }

 private:
  ArgPtrBuffer buffer;
  const ParamList& paramList;
};

class MethodPtr {
 public:
  using MemberVariableFunction = Destructor(void*, void*, ArgsView);
  using MemberConstFunction = Destructor(const void*, void*, ArgsView);
  using StaticFunction = Destructor(void*, ArgsView);

  MethodPtr(MemberVariableFunction* func, ResultDesc resultDesc = {},
            ParamList paramList = {})
      : func{func},
        resultDesc{std::move(resultDesc)},
        paramList{std::move(paramList)} {
    assert(func);
  }

  MethodPtr(MemberConstFunction* func, ResultDesc resultDesc = {},
            ParamList paramList = {})
      : func{func},
        resultDesc{std::move(resultDesc)},
        paramList{std::move(paramList)} {
    assert(func);
  }

  MethodPtr(StaticFunction* func, ResultDesc resultDesc = {},
            ParamList paramList = {})
      : func{func},
        resultDesc{std::move(resultDesc)},
        paramList{std::move(paramList)} {
    assert(func);
  }

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

  bool IsMemberVariable() const noexcept {
    return func.index() == 0 || func.index() == 3;
  }

  bool IsMemberConst() const noexcept {
    return func.index() == 1 || func.index() == 4;
  }

  bool IsStatic() const noexcept {
    return func.index() == 2 || func.index() == 5;
  }

  const ParamList& GetParamList() const noexcept { return paramList; }

  const ResultDesc& GetResultDesc() const noexcept { return resultDesc; }

  bool IsDistinguishableWith(const MethodPtr& rhs) const noexcept {
    return func.index() != rhs.func.index() || paramList != rhs.paramList;
  }

  Destructor Invoke(void* obj, void* result_buffer,
                    ArgPtrBuffer argptr_buffer) const;
  Destructor Invoke(const void* obj, void* result_buffer,
                    ArgPtrBuffer argptr_buffer) const;
  Destructor Invoke(void* result_buffer, ArgPtrBuffer argptr_buffer) const;

 private:
  std::variant<MemberVariableFunction*, MemberConstFunction*, StaticFunction*,
               std::function<MemberVariableFunction>,
               std::function<MemberConstFunction>,
               std::function<StaticFunction>>
      func;
  ResultDesc resultDesc;
  ParamList paramList;
};
}  // namespace My::MyDRefl
