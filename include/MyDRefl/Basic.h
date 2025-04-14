#pragma once

#include "Util.h"

#include <MyTemplate/Type.h>

#include <cassert>
#include <memory>

namespace My::MyDRefl {
enum class MethodFlag {
  Variable = 0b001,
  Const = 0b010,
  Static = 0b100,

  None = 0b000,
  All = 0b111
};
MY_MYDREFL_ENUM_BOOL_OPERATOR_DEFINE(MethodFlag)

enum class FieldFlag {
  Basic = 0b00001,
  Virtual = 0b00010,
  Static = 0b00100,
  DynamicShared = 0b01000,
  DynamicBuffer = 0b10000,

  None = 0b00000,
  Owned = 0b00011,
  Unowned = 0b11100,
  All = 0b11111
};
MY_MYDREFL_ENUM_BOOL_OPERATOR_DEFINE(FieldFlag)

using SharedBuffer = std::shared_ptr<void>;
class ObjectView;
class SharedObject;

template <typename T>
struct IsObjectOrView {
 private:
  using U = std::remove_cvref_t<T>;

 public:
  static constexpr bool value =
      std::is_same_v<U, ObjectView> || std::is_same_v<U, SharedObject>;
};

template <typename T>
constexpr bool IsObjectOrView_v = IsObjectOrView<T>::value;
template <typename T>
concept NonObjectAndView = !IsObjectOrView_v<T>;

template <typename T>
T MoveResult(Type type, void* result_buffer) noexcept(
    std::is_reference_v<T> || std::is_nothrow_destructible_v<T> &&
                                  std::is_nothrow_move_constructible_v<T>) {
  if constexpr (!std::is_void_v<T>) {
    assert(result_buffer && type);

    if constexpr (!std::is_reference_v<T> &&
                  std::is_default_constructible_v<T>) {
      if (type != Type_of<T>)
        return {};
    } else
      assert(type == Type_of<T>);

    if constexpr (std::is_reference_v<T>)
      return std::forward<T>(*buffer_as<std::add_pointer_t<T>>(result_buffer));
    else {
      T rst = std::move(buffer_as<T>(result_buffer));
      if constexpr (std::is_compound_v<T> &&
                    !std::is_trivially_destructible_v<T>)
        reinterpret_cast<const T*>(result_buffer)->~T();
      return rst;
    }
  }
}

struct TypeInfo;
struct FieldInfo;
struct MethodInfo;

struct InfoTypePair {
  Type type;
  TypeInfo* info;
};

struct InfoFieldPair {
  Name name;
  FieldInfo* info;
};

struct InfoMethodPair {
  Name name;
  MethodInfo* info;
};

struct InfoTypeFieldPair {
  InfoTypePair type;
  InfoFieldPair field;
};

struct InfoTypeMethodPair {
  InfoTypePair type;
  InfoMethodPair method;
};
}  // namespace My::MyDRefl
