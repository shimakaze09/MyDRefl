//
// Created by Admin on 3/04/2025.
//

#pragma once

#include <array>

namespace My::MyDRefl {
template <typename T>
T InvokeResult::Move(void* result_buffer) {
  assert(result_buffer);

  if constexpr (!std::is_lvalue_reference_v<T> &&
                std::is_default_constructible_v<std::remove_reference_t<T>>) {
    if (!success)
      return std::forward<T>(T{});
  } else
    assert(success);

  assert(resultID = TypeID::of<T>);

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

template <typename... Args>
bool ConstObjectPtr::IsInvocable(StrID methodID) const noexcept {
  std::array argTypeIDs = {TypeID::of<Args>...};
  return IsInvocable(ID, methodID, Span<TypeID>{argTypeIDs});
}

template <typename T>
T ConstObjectPtr::InvokeRet(StrID methodID, Span<TypeID> argTypeIDs,
                            void* args_buffer) const {
  std::uint8_t result_buffer[sizeof(T)];
  auto result = Invoke(methodID, argTypeIDs, args_buffer, result_buffer);
  assert(result.resultID == TypeID::of<T>);
  return result.Move<T>(result_buffer);
}

template <typename... Args>
InvokeResult ConstObjectPtr::InvokeArgs(StrID methodID, void* result_buffer,
                                        Args... args) const {
  if constexpr (sizeof...(Args) > 0) {
    std::array argTypeIDs = {TypeID::of<Args>...};
    auto args_buffer =
        type_buffer_decay_as_tuple<Args...>(std::forward<Args>(args)...);
    return Invoke(methodID, Span<TypeID>{argTypeIDs},
                  static_cast<void*>(&args_buffer), result_buffer);
  } else
    return Invoke(methodID, {}, nullptr, result_buffer);
}

template <typename T, typename... Args>
T ConstObjectPtr::Invoke(StrID methodID, Args... args) const {
  if constexpr (sizeof...(Args) > 0) {
    std::array argTypeIDs = {TypeID::of<Args>...};
    auto args_buffer =
        type_buffer_decay_as_tuple<Args...>(std::forward<Args>(args)...);
    return InvokeRet<T>(methodID, Span<TypeID>{argTypeIDs},
                        static_cast<void*>(&args_buffer));
  } else
    return InvokeRet<T>(methodID);
}

template <typename... Args>
bool ObjectPtr::IsInvocable(StrID methodID) const noexcept {
  std::array argTypeIDs = {TypeID::of<Args>...};
  return IsInvocable(ID, methodID, Span<TypeID>{argTypeIDs});
}

template <typename T>
T ObjectPtr::InvokeRet(StrID methodID, Span<TypeID> argTypeIDs,
                       void* args_buffer) const {
  std::uint8_t result_buffer[sizeof(T)];
  auto result = Invoke(methodID, argTypeIDs, args_buffer, result_buffer);
  assert(result.resultID == TypeID::of<T>);
  return result.Move<T>(result_buffer);
}

template <typename... Args>
InvokeResult ObjectPtr::InvokeArgs(StrID methodID, void* result_buffer,
                                   Args... args) const {
  if constexpr (sizeof...(Args) > 0) {
    std::array argTypeIDs = {TypeID::of<Args>...};
    auto args_buffer =
        type_buffer_decay_as_tuple<Args...>(std::forward<Args>(args)...);
    return Invoke(methodID, Span<TypeID>{argTypeIDs},
                  static_cast<void*>(&args_buffer), result_buffer);
  } else
    return Invoke(methodID, {}, nullptr, result_buffer);
}

template <typename T, typename... Args>
T ObjectPtr::Invoke(StrID methodID, Args... args) const {
  if constexpr (sizeof...(Args) > 0) {
    std::array argTypeIDs = {TypeID::of<Args>...};
    auto args_buffer =
        type_buffer_decay_as_tuple<Args...>(std::forward<Args>(args)...);
    return InvokeRet<T>(methodID, Span<TypeID>{argTypeIDs},
                        static_cast<void*>(&args_buffer));
  } else
    return InvokeRet<T>(methodID);
}
}  // namespace My::MyDRefl

template <>
struct std::hash<My::MyDRefl::SharedObject> {
  std::size_t operator()(const My::MyDRefl::SharedObject& obj) const noexcept {
    return obj.GetID().GetValue() ^ std::hash<const void*>()(obj.GetPtr());
  }
};

inline bool operator==(const My::MyDRefl::SharedObject& left,
                       const My::MyDRefl::SharedObject& right) noexcept {
  return left.GetID() == right.GetID() && left.GetPtr() == right.GetPtr();
}

inline bool operator!=(const My::MyDRefl::SharedObject& left,
                       const My::MyDRefl::SharedObject& right) noexcept {
  return left.GetID() != right.GetID() || left.GetPtr() != right.GetPtr();
}

inline bool operator<(const My::MyDRefl::SharedObject& left,
                      const My::MyDRefl::SharedObject& right) noexcept {
  return left.GetID() < right.GetID() ||
         (left.GetID() == right.GetID() && left.GetPtr() < right.GetPtr());
}

inline bool operator>=(const My::MyDRefl::SharedObject& left,
                       const My::MyDRefl::SharedObject& right) noexcept {
  return left.GetID() > right.GetID() ||
         (left.GetID() == right.GetID() && left.GetPtr() >= right.GetPtr());
}

inline bool operator>(const My::MyDRefl::SharedObject& left,
                      const My::MyDRefl::SharedObject& right) noexcept {
  return left.GetID() > right.GetID() ||
         (left.GetID() == right.GetID() && left.GetPtr() > right.GetPtr());
}

inline bool operator<=(const My::MyDRefl::SharedObject& left,
                       const My::MyDRefl::SharedObject& right) noexcept {
  return left.GetID() < right.GetID() ||
         (left.GetID() == right.GetID() && left.GetPtr() <= right.GetPtr());
}

template <class Elem, typename Traits>
std::basic_ostream<Elem, Traits>& operator<<(
    std::basic_ostream<Elem, Traits>& out,
    const My::MyDRefl::SharedObject& obj) {
  return out << obj.GetID().GetValue() << obj.GetPtr();
}

namespace std {
inline void swap(My::MyDRefl::SharedObject& left,
                 My::MyDRefl::SharedObject& right) noexcept {
  left.Swap(right);
}
}  // namespace std
