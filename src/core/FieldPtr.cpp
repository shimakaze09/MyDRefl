//
// Created by Admin on 3/04/2025.
//

#include <MyDRefl/FieldPtr.h>

using namespace My::MyDRefl;

FieldPtr::FieldPtr(TypeID valueID, size_t forward_offset_value,
                   bool isConst) noexcept
    : valueID{valueID} {
  assert(valueID);

  if (isConst)
    data.emplace<1>(forward_offset_value);
  else
    data.emplace<0>(forward_offset_value);
}

FieldPtr::FieldPtr(TypeID valueID, const Buffer& buffer, bool isConst) noexcept
    : valueID{valueID} {
  assert(valueID);

  if (isConst)
    data.emplace<9>(buffer);
  else
    data.emplace<8>(buffer);
}

ObjectPtr FieldPtr::Map() noexcept {
  return std::visit(
      [this](auto& value) -> ObjectPtr {
        using T = std::remove_reference_t<decltype(value)>;
        if constexpr (std::is_same_v<T, size_t>) {
          assert(false);
          return nullptr;
        } else if constexpr (std::is_same_v<T, Offsetor>) {
          assert(false);
          return nullptr;
        } else if constexpr (std::is_same_v<T, void*>) {
          return {valueID, value};
        } else if constexpr (std::is_same_v<T, const void*>) {
          assert(false);
          return nullptr;
        } else if constexpr (std::is_same_v<T, SharedBuffer>) {
          return {valueID, value.get()};
        } else if constexpr (std::is_same_v<T, SharedConstBuffer>) {
          assert(false);
          return nullptr;
        } else if constexpr (std::is_same_v<T, Buffer>) {
          return {valueID, value.data()};
        } else if constexpr (std::is_same_v<T, const Buffer>) {
          assert(false);
          return nullptr;
        } else
          static_assert(false);
      },
      data);
}

ConstObjectPtr FieldPtr::Map() const noexcept {
  return std::visit(
      [this](const auto& value) -> ConstObjectPtr {
        using T = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<T, size_t>) {
          assert(false);
          return nullptr;
        } else if constexpr (std::is_same_v<T, Offsetor>) {
          assert(false);
          return nullptr;
        } else if constexpr (std::is_same_v<T, void*>) {
          return {valueID, value};
        } else if constexpr (std::is_same_v<T, const void*>) {
          return {valueID, value};
        } else if constexpr (std::is_same_v<T, SharedBuffer>) {
          return {valueID, value.get()};
        } else if constexpr (std::is_same_v<T, SharedConstBuffer>) {
          return {valueID, value.get()};
        } else if constexpr (std::is_same_v<T, Buffer>) {
          return {valueID, value.data()};
        } else if constexpr (std::is_same_v<T, const Buffer>) {
          return {valueID, value.data()};
        } else
          static_assert(false);
      },
      data);
}

ConstObjectPtr FieldPtr::Map(const void* obj) const noexcept {
  return std::visit(
      [this, obj](const auto& value) -> ConstObjectPtr {
        using T = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<T, size_t>) {
          assert(obj);
          return {valueID, forward_offset(obj, value)};
        } else if constexpr (std::is_same_v<T, Offsetor>) {
          assert(obj);
          return {valueID, value(obj)};
        } else if constexpr (std::is_same_v<T, void*>) {
          return {valueID, value};
        } else if constexpr (std::is_same_v<T, const void*>) {
          return {valueID, value};
        } else if constexpr (std::is_same_v<T, SharedBuffer>) {
          return {valueID, value.get()};
        } else if constexpr (std::is_same_v<T, SharedConstBuffer>) {
          return {valueID, value.get()};
        } else if constexpr (std::is_same_v<T, Buffer>) {
          return {valueID, value.data()};
        } else if constexpr (std::is_same_v<T, const Buffer>) {
          return {valueID, value.data()};
        } else
          static_assert(false);
      },
      data);
}

ObjectPtr FieldPtr::Map(void* obj) noexcept {
  switch (data.index()) {
    case 0:
      assert(obj);
      return {valueID, forward_offset(obj, std::get<0>(data))};
    case 2:
      assert(obj);
      return {valueID, const_cast<void*>(std::get<2>(data)(obj))};
    case 4:
      return {valueID, std::get<4>(data)};
    case 6:
      return {valueID, std::get<6>(data).get()};
    case 8:
      return {valueID, std::get<8>(data).data()};
    default:
      assert("require variable" && false);
      return nullptr;
  }
}
