#include <MyDRefl/FieldPtr.h>

using namespace My::MyDRefl;

ObjectView FieldPtr::Var() {
  return std::visit(
      [this]<typename T>(T& value) -> ObjectView {
        if constexpr (std::is_same_v<T, size_t>) {
          assert(false);
          return {};
        } else if constexpr (std::is_same_v<T, Offsetor>) {
          assert(false);
          return {};
        } else if constexpr (std::is_same_v<T, void*>) {
          return {type, value};
        } else if constexpr (std::is_same_v<T, SharedBuffer>) {
          return {type, value.get()};
        } else if constexpr (std::is_same_v<T, Buffer>) {
          return {type, &value};
        } else
          static_assert(always_false<T>);
      },
      data);
}

ObjectView FieldPtr::Var(void* obj) {
  return std::visit(
      [obj, this]<typename T>(T& value) -> ObjectView {
        if constexpr (std::is_same_v<T, size_t>) {
          assert(obj);
          return {type, forward_offset(obj, value)};
        } else if constexpr (std::is_same_v<T, Offsetor>) {
          assert(obj);
          return {type, value(obj)};
        } else if constexpr (std::is_same_v<T, void*>) {
          return {type, value};
        } else if constexpr (std::is_same_v<T, SharedBuffer>) {
          return {type, value.get()};
        } else if constexpr (std::is_same_v<T, Buffer>) {
          return {type, &value};
        } else
          static_assert(always_false<T>);
      },
      data);
}

ObjectView FieldPtr::Var() const {
  return std::visit(
      [this]<typename T>(const T& value) -> ObjectView {
        if constexpr (std::is_same_v<T, size_t>) {
          assert(false);
          return {};
        } else if constexpr (std::is_same_v<T, Offsetor>) {
          assert(false);
          return {};
        } else if constexpr (std::is_same_v<T, void*>) {
          return {type, value};
        } else if constexpr (std::is_same_v<T, SharedBuffer>) {
          return {type, value.get()};
        } else if constexpr (std::is_same_v<T, Buffer>) {
          assert(false);
          return {};
        } else
          static_assert(always_false<T>);
      },
      data);
}

ObjectView FieldPtr::Var(void* obj) const {
  return std::visit(
      [obj, this]<typename T>(const T& value) -> ObjectView {
        if constexpr (std::is_same_v<T, size_t>) {
          assert(obj);
          return {type, forward_offset(obj, value)};
        } else if constexpr (std::is_same_v<T, Offsetor>) {
          assert(obj);
          return {type, value(obj)};
        } else if constexpr (std::is_same_v<T, void*>) {
          return {type, value};
        } else if constexpr (std::is_same_v<T, SharedBuffer>) {
          return {type, value.get()};
        } else if constexpr (std::is_same_v<T, Buffer>) {
          assert(false);
          return {};
        } else
          static_assert(always_false<T>);
      },
      data);
}
