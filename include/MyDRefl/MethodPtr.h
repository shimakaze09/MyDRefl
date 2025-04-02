//
// Created by Admin on 1/04/2025.
//

#pragma once

#include "MyTemplate/Func.h"
#include "Object.h"
#include "Util.h"

#include <MyContainer/Span.h>

#include <any>
#include <functional>
#include <variant>
#include <vector>

namespace My::MyDRefl {
struct Parameter {
  TypeID typeID;
  size_t size;
  size_t alignment;
  NameID nameID;
};

struct ResultDesc {
  TypeID typeID;
  size_t size{0};
  size_t alignment{1};
};

class ParamList {
 public:
  ParamList(std::vector<Parameter> params = {});

  size_t GetBufferSize() const noexcept { return size; }

  size_t GetBufferAlignment() const noexcept { return alignment; }

  const std::vector<size_t>& GetOffsets() const noexcept { return offsets; }

  const std::vector<Parameter>& GetParameters() const noexcept {
    return params;
  }

  bool IsConpatibleWith(Span<TypeID> typeIDs) const noexcept;

 private:
  size_t size{0};
  size_t alignment{1};
  std::vector<size_t> offsets;
  std::vector<Parameter> params;
};

class ArgsView {
 public:
  ArgsView(void* buffer, const ParamList& paramList)
      : buffer{buffer}, paramList{paramList} {}

  void* GetBuffer() const noexcept { return buffer; }

  const ParamList& GetParamList() const noexcept { return paramList; }

  ObjectPtr At(size_t idx) const noexcept {
    assert(idx < paramList.GetParameters().size());
    return {paramList.GetParameters()[idx].typeID,
            forward_offset(buffer, paramList.GetOffsets()[idx])};
  }

 private:
  void* buffer;
  const ParamList& paramList;
};

class MethodPtr {
 public:
  enum class Mode {
    OBJECT_VARIABLE,
    OBJECT_CONST,
    STATIC,
  };

  template <typename T>
  static MethodPtr GenerateDefaultConstructor() {
    return {static_cast<ObjectVariableFunction*>(
        [](void* obj, ArgsView, void*) -> Destructor* {
          new (obj) T;
          return nullptr;
        })};
  }

  template <typename T>
  static MethodPtr GenerateDestructor() {
    return {static_cast<ObjectConstFunction*>(
        [](const void* obj, ArgsView, void*) -> Destructor* {
          reinterpret_cast<const T*>(obj)->~T();
          return nullptr;
        })};
  }

  using ObjectVariableFunction = std::add_pointer_t<Destructor>(void*, ArgsView,
                                                                void*);
  using ObjectConstFunction = std::add_pointer_t<Destructor>(const void*,
                                                             ArgsView, void*);
  using StaticFunction = std::add_pointer_t<Destructor>(ArgsView, void*);

  MethodPtr(ObjectVariableFunction* func, ResultDesc resultDesc = {},
            ParamList paramList = {}) noexcept
      : mode{Mode::OBJECT_VARIABLE},
        func_object_variable{func},
        resultDesc{std::move(resultDesc)},
        paramList{std::move(paramList)} {
    assert(func);
  }

  MethodPtr(ObjectConstFunction* func, ResultDesc resultDesc = {},
            ParamList paramList = {}) noexcept
      : mode{Mode::OBJECT_CONST},
        func_object_const{func},
        resultDesc{std::move(resultDesc)},
        paramList{std::move(paramList)} {
    assert(func);
  }

  MethodPtr(StaticFunction* func, ResultDesc resultDesc = {},
            ParamList paramList = {}) noexcept
      : mode{Mode::STATIC},
        func_static{func},
        resultDesc{std::move(resultDesc)},
        paramList{std::move(paramList)} {
    assert(func);
  }

  template <typename Lambda,
            std::enable_if_t<!std::is_member_pointer_v<Lambda> &&
                                 !std::is_pointer_v<Lambda>,
                             int> = 0>
  MethodPtr(Lambda func, ResultDesc resultDesc = {},
            ParamList paramList = {}) noexcept
      : MethodPtr{DecayLambda(func), std::move(resultDesc),
                  std::move(paramList)} {}

  Mode GetMode() const noexcept { return mode; }

  const ParamList& GetParamList() const noexcept { return paramList; }

  const ResultDesc& GetResultDesc() const noexcept { return resultDesc; }

  Destructor* Invoke(void* obj, void* args_buffer, void* result_buffer) const {
    ArgsView args = {args_buffer, paramList};
    switch (mode) {
      case Mode::OBJECT_VARIABLE:
        return func_object_variable(obj, args, result_buffer);
      case Mode::OBJECT_CONST:
        return func_object_const(obj, args, result_buffer);
      case Mode::STATIC:
        return func_static(args, result_buffer);
      default:
        assert(false);
        return {};
    }
  };

  Destructor* Invoke(const void* obj, void* args_buffer,
                     void* result_buffer) const {
    ArgsView args = {args_buffer, paramList};
    switch (mode) {
      case Mode::OBJECT_CONST:
        return func_object_const(obj, args, result_buffer);
      case Mode::STATIC:
        return func_static(args, result_buffer);
      default:
        assert(false);
        return nullptr;
    }
  };

  Destructor* Invoke(void* args_buffer, void* result_buffer) const {
    ArgsView args = {args_buffer, paramList};
    switch (mode) {
      case Mode::STATIC:
        return func_static(args, result_buffer);
      default:
        assert(false);
        return nullptr;
    }
  };

  Destructor* Invoke_Static(void* args_buffer, void* result_buffer) const {
    assert(mode == Mode::STATIC);
    return func_static({args_buffer, paramList}, result_buffer);
  };

 private:
  Mode mode;

  union {
    ObjectVariableFunction* func_object_variable;
    ObjectConstFunction* func_object_const;
    StaticFunction* func_static;
  };

  ResultDesc resultDesc;
  ParamList paramList;
};

struct InvokeResult {
  bool success{false};
  TypeID resultID;
  Destructor* destructor{nullptr};

  template <typename T>
  T Move(void* result_buffer) {
    if constexpr (!std::is_lvalue_reference_v<T> &&
                  std::is_default_constructible_v<std::remove_reference_t<T>>) {
      if (!success)
        return std::forward<T>(T{});
    } else
      assert(success);

    // assert(resultID = TypeRegistry::DirectGetID<T>());

    if constexpr (std::is_lvalue_reference_v<T>) {
      using PtrT = std::add_pointer_t<std::remove_reference_t<T>>;
      assert(destructor == nullptr);
      return *buffer_as<PtrT>(result_buffer);
    } else {
      T rst = std::move(buffer_as<T>(result_buffer));
      if (destructor)
        destructor(result_buffer);
      return rst;
    }
  }
};
}  // namespace My::MyDRefl
