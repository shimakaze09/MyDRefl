//
// Created by Admin on 3/04/2025.
//

#pragma once

#include <array>

namespace My::MyDRefl {
//
// ObjectPtrBase
//////////////////

template <typename... Args>
InvocableResult ObjectPtrBase::IsInvocable(StrID methodID) const noexcept {
  std::array argTypeIDs = {TypeID::of<Args>...};
  return IsInvocable(ID, methodID, Span<const TypeID>{argTypeIDs});
}

template <typename T>
T ObjectPtrBase::InvokeRet(StrID methodID, Span<const TypeID> argTypeIDs,
                           void* args_buffer) const {
  using U =
      std::conditional_t<std::is_reference_v<T>, std::add_pointer_t<T>, T>;
  std::uint8_t result_buffer[sizeof(U)];
  auto result = Invoke(methodID, result_buffer, argTypeIDs, args_buffer);
  assert(result.resultID == TypeID::of<T>);
  return result.Move<T>(result_buffer);
}

template <typename... Args>
InvokeResult ObjectPtrBase::InvokeArgs(StrID methodID, void* result_buffer,
                                       Args... args) const {
  if constexpr (sizeof...(Args) > 0) {
    static_assert(
        !((std::is_const_v<Args> || std::is_volatile_v<Args>) || ...));
    std::array argTypeIDs = {TypeID::of<Args>...};
    std::array args_buffer{reinterpret_cast<std::size_t>(&args)...};
    return Invoke(methodID, Span<const TypeID>{argTypeIDs},
                  static_cast<void*>(args_buffer.data()), result_buffer);
  } else
    return Invoke(methodID, {}, nullptr, result_buffer);
}

template <typename T, typename... Args>
T ObjectPtrBase::Invoke(StrID methodID, Args... args) const {
  if constexpr (sizeof...(Args) > 0) {
    static_assert(
        !((std::is_const_v<Args> || std::is_volatile_v<Args>) || ...));
    std::array argTypeIDs = {TypeID::of<Args>...};
    std::array args_buffer{reinterpret_cast<std::size_t>(&args)...};
    return InvokeRet<T>(methodID, Span<const TypeID>{argTypeIDs},
                        static_cast<void*>(args_buffer.data()));
  } else
    return InvokeRet<T>(methodID);
}

template <typename... Args>
SharedObject ObjectPtrBase::MInvoke(StrID methodID,
                                    std::pmr::memory_resource* rst_rsrc,
                                    Args... args) const {
  if constexpr (sizeof...(Args) > 0) {
    static_assert(
        !((std::is_const_v<Args> || std::is_volatile_v<Args>) || ...));
    std::array argTypeIDs = {TypeID::of<Args>...};
    std::array args_buffer{reinterpret_cast<std::size_t>(&args)...};
    return MInvoke(methodID, Span<const TypeID>{argTypeIDs},
                   static_cast<void*>(args_buffer.data()), rst_rsrc);
  } else
    return MInvoke(methodID, Span<const TypeID>{}, static_cast<void*>(nullptr),
                   rst_rsrc);
}

template <typename... Args>
SharedObject ObjectPtrBase::DMInvoke(StrID methodID, Args... args) const {
  return MInvoke<Args...>(methodID, std::pmr::get_default_resource(),
                          std::forward<Args>(args)...);
}

template <typename... Args>
SharedObject ObjectPtrBase::AMInvoke(StrID methodID,
                                     std::pmr::memory_resource* rst_rsrc,
                                     Args... args) const {
  if constexpr (sizeof...(Args) > 0) {
    static_assert(
        !((std::is_const_v<Args> || std::is_volatile_v<Args>) || ...));
    std::array argTypeIDs = {ArgID<Args>(std::forward<Args>(args))...};
    std::array args_buffer{reinterpret_cast<std::size_t>(ArgPtr(args))...};
    return MInvoke(methodID, Span<const TypeID>{argTypeIDs},
                   static_cast<void*>(args_buffer.data()), rst_rsrc);
  } else
    return MInvoke(methodID, Span<const TypeID>{}, static_cast<void*>(nullptr),
                   rst_rsrc);
}

template <typename... Args>
SharedObject ObjectPtrBase::ADMInvoke(StrID methodID, Args... args) const {
  return AMInvoke<Args...>(methodID, std::pmr::get_default_resource(),
                           std::forward<Args>(args)...);
}

//
// ObjectPtr
//////////////

template <typename... Args>
InvocableResult ObjectPtr::IsInvocable(StrID methodID) const noexcept {
  std::array argTypeIDs = {TypeID::of<Args>...};
  return IsInvocable(ID, methodID, Span<const TypeID>{argTypeIDs});
}

template <typename T>
T ObjectPtr::InvokeRet(StrID methodID, Span<const TypeID> argTypeIDs,
                       void* args_buffer) const {
  if constexpr (!std::is_void_v<T>) {
    using U =
        std::conditional_t<std::is_reference_v<T>, std::add_pointer_t<T>, T>;
    std::uint8_t result_buffer[sizeof(U)];
    auto result = Invoke(methodID, result_buffer, argTypeIDs, args_buffer);
    assert(result.resultID == TypeID::of<T>);
    return result.Move<T>(result_buffer);
  } else
    Invoke(methodID, nullptr, argTypeIDs, args_buffer);
}

template <typename... Args>
InvokeResult ObjectPtr::InvokeArgs(StrID methodID, void* result_buffer,
                                   Args... args) const {
  if constexpr (sizeof...(Args) > 0) {
    static_assert(
        !((std::is_const_v<Args> || std::is_volatile_v<Args>) || ...));
    std::array argTypeIDs = {TypeID::of<Args>...};
    std::array args_buffer{reinterpret_cast<std::size_t>(&args)...};
    return Invoke(methodID, Span<const TypeID>{argTypeIDs},
                  static_cast<void*>(args_buffer.data()), result_buffer);
  } else
    return Invoke(methodID, {}, nullptr, result_buffer);
}

template <typename T, typename... Args>
T ObjectPtr::Invoke(StrID methodID, Args... args) const {
  if constexpr (sizeof...(Args) > 0) {
    static_assert(
        !((std::is_const_v<Args> || std::is_volatile_v<Args>) || ...));
    std::array argTypeIDs = {TypeID::of<Args>...};
    std::array args_buffer{reinterpret_cast<std::size_t>(&args)...};
    return InvokeRet<T>(methodID, Span<const TypeID>{argTypeIDs},
                        static_cast<void*>(args_buffer.data()));
  } else
    return InvokeRet<T>(methodID);
}

template <typename... Args>
SharedObject ObjectPtr::MInvoke(StrID methodID,
                                std::pmr::memory_resource* rst_rsrc,
                                Args... args) const {
  if constexpr (sizeof...(Args) > 0) {
    static_assert(
        !((std::is_const_v<Args> || std::is_volatile_v<Args>) || ...));
    std::array argTypeIDs = {TypeID::of<Args>...};
    std::array args_buffer{reinterpret_cast<std::size_t>(&args)...};
    return MInvoke(methodID, Span<const TypeID>{argTypeIDs},
                   static_cast<void*>(args_buffer.data()), rst_rsrc);
  } else
    return MInvoke(methodID, Span<const TypeID>{}, static_cast<void*>(nullptr),
                   rst_rsrc);
}

template <typename... Args>
SharedObject ObjectPtr::DMInvoke(StrID methodID, Args... args) const {
  return MInvoke<Args...>(methodID, std::pmr::get_default_resource(),
                          std::forward<Args>(args)...);
}

template <typename... Args>
SharedObject ObjectPtr::AMInvoke(StrID methodID,
                                 std::pmr::memory_resource* rst_rsrc,
                                 Args... args) const {
  if constexpr (sizeof...(Args) > 0) {
    static_assert(
        !((std::is_const_v<Args> || std::is_volatile_v<Args>) || ...));
    std::array argTypeIDs = {ArgID<Args>(std::forward<Args>(args))...};
    std::array args_buffer{reinterpret_cast<std::size_t>(ArgPtr(args))...};
    return MInvoke(methodID, Span<const TypeID>{argTypeIDs},
                   static_cast<void*>(args_buffer.data()), rst_rsrc);
  } else
    return MInvoke(methodID, Span<const TypeID>{}, static_cast<void*>(nullptr),
                   rst_rsrc);
}

template <typename... Args>
SharedObject ObjectPtr::ADMInvoke(StrID methodID, Args... args) const {
  return AMInvoke<Args...>(methodID, std::pmr::get_default_resource(),
                           std::forward<Args>(args)...);
}
}  // namespace My::MyDRefl

template <>
struct std::hash<My::MyDRefl::ObjectPtr> {
  std::size_t operator()(const My::MyDRefl::ObjectPtr& obj) const noexcept {
    return obj.GetID().GetValue() ^ std::hash<const void*>()(obj.GetPtr());
  }
};

template <>
struct std::hash<My::MyDRefl::ConstObjectPtr> {
  std::size_t operator()(
      const My::MyDRefl::ConstObjectPtr& obj) const noexcept {
    return obj.GetID().GetValue() ^ std::hash<const void*>()(obj.GetPtr());
  }
};

template <>
struct std::hash<My::MyDRefl::SharedObject> {
  std::size_t operator()(const My::MyDRefl::SharedObject& obj) const noexcept {
    return obj.GetID().GetValue() ^ std::hash<const void*>()(obj.GetPtr());
  }
};

template <>
struct std::hash<My::MyDRefl::SharedConstObject> {
  std::size_t operator()(
      const My::MyDRefl::SharedConstObject& obj) const noexcept {
    return obj.GetID().GetValue() ^ std::hash<const void*>()(obj.GetPtr());
  }
};

namespace std {
inline void swap(My::MyDRefl::SharedObject& left,
                 My::MyDRefl::SharedObject& right) noexcept {
  left.Swap(right);
}

inline void swap(My::MyDRefl::SharedConstObject& left,
                 My::MyDRefl::SharedConstObject& right) noexcept {
  left.Swap(right);
}
}  // namespace std

template <typename T>
struct My::MyDRefl::IsObjectOrPtr {
 private:
  using U = std::remove_cv_t<T>;

 public:
  static constexpr bool value =
      std::is_same_v<T, ObjectPtr> || std::is_same_v<T, ConstObjectPtr> ||
      std::is_same_v<T, SharedObject> || std::is_same_v<T, SharedConstObject>;
};

template <typename T,
          std::enable_if_t<!My::MyDRefl::IsObjectOrPtr_v<T>, int> = 0>
bool operator==(const T& lhs, My::MyDRefl::ConstObjectPtr ptr) {
  return My::MyDRefl::Ptr(lhs) == My::MyDRefl::ConstCast(ptr);
}

template <typename T,
          std::enable_if_t<!My::MyDRefl::IsObjectOrPtr_v<T>, int> = 0>
bool operator!=(const T& lhs, My::MyDRefl::ConstObjectPtr ptr) {
  return My::MyDRefl::Ptr(lhs) != My::MyDRefl::ConstCast(ptr);
}

template <typename T,
          std::enable_if_t<!My::MyDRefl::IsObjectOrPtr_v<T>, int> = 0>
bool operator<(const T& lhs, My::MyDRefl::ConstObjectPtr ptr) {
  return My::MyDRefl::Ptr(lhs) < My::MyDRefl::ConstCast(ptr);
}

template <typename T,
          std::enable_if_t<!My::MyDRefl::IsObjectOrPtr_v<T>, int> = 0>
bool operator>(const T& lhs, My::MyDRefl::ConstObjectPtr ptr) {
  return My::MyDRefl::Ptr(lhs) > My::MyDRefl::ConstCast(ptr);
}

template <typename T,
          std::enable_if_t<!My::MyDRefl::IsObjectOrPtr_v<T>, int> = 0>
bool operator<=(const T& lhs, My::MyDRefl::ConstObjectPtr ptr) {
  return My::MyDRefl::Ptr(lhs) <= My::MyDRefl::ConstCast(ptr);
}

template <typename T,
          std::enable_if_t<!My::MyDRefl::IsObjectOrPtr_v<T>, int> = 0>
bool operator>=(const T& lhs, My::MyDRefl::ConstObjectPtr ptr) {
  return My::MyDRefl::Ptr(lhs) >= My::MyDRefl::ConstCast(ptr);
}

template <typename T,
          std::enable_if_t<!My::MyDRefl::IsObjectOrPtr_v<T>, int> = 0>
T& operator<<(T& lhs, My::MyDRefl::ConstObjectPtr ptr) {
  return ptr >> lhs;
}

//template<typename T, std::enable_if_t<!My::MyDRefl::IsObjectOrPtr_v<T>, int> = 0>
//My::MyDRefl::SharedObject operator>>(const T& lhs, My::MyDRefl::ConstObjectPtr ptr) {
//	return ptr << lhs;
//}

template <typename T,
          std::enable_if_t<!My::MyDRefl::IsObjectOrPtr_v<T>, int> = 0>
bool operator==(const T& lhs, My::MyDRefl::ObjectPtr ptr) {
  return My::MyDRefl::Ptr(lhs) == ptr;
}

template <typename T,
          std::enable_if_t<!My::MyDRefl::IsObjectOrPtr_v<T>, int> = 0>
bool operator!=(const T& lhs, My::MyDRefl::ObjectPtr ptr) {
  return My::MyDRefl::Ptr(lhs) != ptr;
}

template <typename T,
          std::enable_if_t<!My::MyDRefl::IsObjectOrPtr_v<T>, int> = 0>
bool operator<(const T& lhs, My::MyDRefl::ObjectPtr ptr) {
  return My::MyDRefl::Ptr(lhs) < ptr;
}

template <typename T,
          std::enable_if_t<!My::MyDRefl::IsObjectOrPtr_v<T>, int> = 0>
bool operator>(const T& lhs, My::MyDRefl::ObjectPtr ptr) {
  return My::MyDRefl::Ptr(lhs) > ptr;
}

template <typename T,
          std::enable_if_t<!My::MyDRefl::IsObjectOrPtr_v<T>, int> = 0>
bool operator<=(const T& lhs, My::MyDRefl::ObjectPtr ptr) {
  return My::MyDRefl::Ptr(lhs) <= ptr;
}

template <typename T,
          std::enable_if_t<!My::MyDRefl::IsObjectOrPtr_v<T>, int> = 0>
bool operator>=(const T& lhs, My::MyDRefl::ObjectPtr ptr) {
  return My::MyDRefl::Ptr(lhs) >= ptr;
}

template <typename T,
          std::enable_if_t<!My::MyDRefl::IsObjectOrPtr_v<T>, int> = 0>
T& operator<<(T& lhs, My::MyDRefl::ObjectPtr ptr) {
  return ptr >> lhs;
}

template <typename T,
          std::enable_if_t<!My::MyDRefl::IsObjectOrPtr_v<T>, int> = 0>
My::MyDRefl::SharedObject operator>>(const T& lhs, My::MyDRefl::ObjectPtr ptr) {
  return ptr << lhs;
}

template <typename T,
          std::enable_if_t<!My::MyDRefl::IsObjectOrPtr_v<T>, int> = 0>
bool operator==(const T& lhs, const My::MyDRefl::SharedConstObject& ptr) {
  return My::MyDRefl::Ptr(lhs) == My::MyDRefl::ConstCast(ptr);
}

template <typename T,
          std::enable_if_t<!My::MyDRefl::IsObjectOrPtr_v<T>, int> = 0>
bool operator!=(const T& lhs, const My::MyDRefl::SharedConstObject& ptr) {
  return My::MyDRefl::Ptr(lhs) != My::MyDRefl::ConstCast(ptr);
}

template <typename T,
          std::enable_if_t<!My::MyDRefl::IsObjectOrPtr_v<T>, int> = 0>
bool operator<(const T& lhs, const My::MyDRefl::SharedConstObject& ptr) {
  return My::MyDRefl::Ptr(lhs) < My::MyDRefl::ConstCast(ptr);
}

template <typename T,
          std::enable_if_t<!My::MyDRefl::IsObjectOrPtr_v<T>, int> = 0>
bool operator>(const T& lhs, const My::MyDRefl::SharedConstObject& ptr) {
  return My::MyDRefl::Ptr(lhs) > My::MyDRefl::ConstCast(ptr);
}

template <typename T,
          std::enable_if_t<!My::MyDRefl::IsObjectOrPtr_v<T>, int> = 0>
bool operator<=(const T& lhs, const My::MyDRefl::SharedConstObject& ptr) {
  return My::MyDRefl::Ptr(lhs) <= My::MyDRefl::ConstCast(ptr);
}

template <typename T,
          std::enable_if_t<!My::MyDRefl::IsObjectOrPtr_v<T>, int> = 0>
bool operator>=(const T& lhs, const My::MyDRefl::SharedConstObject& ptr) {
  return My::MyDRefl::Ptr(lhs) >= My::MyDRefl::ConstCast(ptr);
}

template <typename T,
          std::enable_if_t<!My::MyDRefl::IsObjectOrPtr_v<T>, int> = 0>
T& operator<<(T& lhs, const My::MyDRefl::SharedConstObject& ptr) {
  return ptr >> lhs;
}

//template<typename T, std::enable_if_t<!My::MyDRefl::IsObjectOrPtr_v<T>, int> = 0>
//My::MyDRefl::SharedObject operator>>(const T& lhs, const My::MyDRefl::SharedConstObject& ptr) {
//	return ptr << lhs;
//}

template <typename T,
          std::enable_if_t<!My::MyDRefl::IsObjectOrPtr_v<T>, int> = 0>
bool operator==(const T& lhs, const My::MyDRefl::SharedObject& ptr) {
  return My::MyDRefl::Ptr(lhs) == ptr;
}

template <typename T,
          std::enable_if_t<!My::MyDRefl::IsObjectOrPtr_v<T>, int> = 0>
bool operator!=(const T& lhs, const My::MyDRefl::SharedObject& ptr) {
  return My::MyDRefl::Ptr(lhs) != ptr;
}

template <typename T,
          std::enable_if_t<!My::MyDRefl::IsObjectOrPtr_v<T>, int> = 0>
bool operator<(const T& lhs, const My::MyDRefl::SharedObject& ptr) {
  return My::MyDRefl::Ptr(lhs) < ptr;
}

template <typename T,
          std::enable_if_t<!My::MyDRefl::IsObjectOrPtr_v<T>, int> = 0>
bool operator>(const T& lhs, const My::MyDRefl::SharedObject& ptr) {
  return My::MyDRefl::Ptr(lhs) > ptr;
}

template <typename T,
          std::enable_if_t<!My::MyDRefl::IsObjectOrPtr_v<T>, int> = 0>
bool operator<=(const T& lhs, const My::MyDRefl::SharedObject& ptr) {
  return My::MyDRefl::Ptr(lhs) <= ptr;
}

template <typename T,
          std::enable_if_t<!My::MyDRefl::IsObjectOrPtr_v<T>, int> = 0>
bool operator>=(const T& lhs, const My::MyDRefl::SharedObject& ptr) {
  return My::MyDRefl::Ptr(lhs) >= ptr;
}

template <typename T,
          std::enable_if_t<!My::MyDRefl::IsObjectOrPtr_v<T>, int> = 0>
T& operator<<(T& lhs, const My::MyDRefl::SharedObject& ptr) {
  return ptr >> lhs;
}

template <typename T,
          std::enable_if_t<!My::MyDRefl::IsObjectOrPtr_v<T>, int> = 0>
My::MyDRefl::SharedObject operator>>(const T& lhs,
                                     const My::MyDRefl::SharedObject& ptr) {
  return ptr << lhs;
}
