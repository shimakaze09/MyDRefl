//
// Created by Admin on 3/04/2025.
//

#pragma once

#include <array>

#define OBJECT_PTR_DEFINE_OPERATOR_T(type, op, name)              \
  template <typename Arg>                                         \
  SharedObject type::operator op(Arg rhs) const {                 \
    return ADMInvoke<Arg>(StrIDRegistry::MetaID::operator_##name, \
                          std::forward<Arg>(rhs));                \
  }

#define OBJECT_PTR_DEFINE_CONTAINER_T(type, name)                  \
  template <typename Arg>                                          \
  SharedObject type::name(Arg rhs) const {                         \
    return ADMInvoke<Arg>(StrIDRegistry::MetaID::container_##name, \
                          std::forward<Arg>(rhs));                 \
  }

#define OBJECT_PTR_DEFINE_CONTAINER_VARS_T(type, name)                 \
  template <typename... Args>                                          \
  SharedObject type::name(Args... args) const {                        \
    return ADMInvoke<Args...>(StrIDRegistry::MetaID::container_##name, \
                              std::forward<Args>(args)...);            \
  }

#define SHARED_OBJECT_DEFINE_OPERATOR_T(type, op)                   \
  template <typename Arg>                                           \
  SharedObject type::operator op(Arg rhs) const {                   \
    return AsObjectPtr()->operator op<Arg>(std::forward<Arg>(rhs)); \
  }

#define DEFINE_OPERATOR_LSHIFT(Lhs, Rhs)             \
  inline Lhs& operator<<(Lhs& lhs, const Rhs& rhs) { \
    return rhs >> lhs;                               \
  }

#define DEFINE_OPERATOR_RSHIFT(Lhs, Rhs)             \
  inline Lhs& operator>>(Lhs& lhs, const Rhs& rhs) { \
    rhs << lhs;                                      \
    return lhs;                                      \
  }

namespace My::MyDRefl {
//
// ObjectPtrBase
//////////////////

template <typename... Args>
InvocableResult ObjectPtrBase::IsInvocable(StrID methodID) const {
  constexpr std::array argTypeIDs = {TypeID_of<Args>...};
  return IsInvocable(methodID, Span<const TypeID>{argTypeIDs});
}

template <typename T>
T ObjectPtrBase::InvokeRet(StrID methodID, Span<const TypeID> argTypeIDs,
                           void* args_buffer) const {
  using U =
      std::conditional_t<std::is_reference_v<T>, std::add_pointer_t<T>, T>;
  std::uint8_t result_buffer[sizeof(U)];
  InvokeResult result =
      Invoke(methodID, result_buffer, argTypeIDs, args_buffer);
  assert(result.resultID == TypeID_of<T>);
  return result.Move<T>(result_buffer);
}

template <typename... Args>
InvokeResult ObjectPtrBase::InvokeArgs(StrID methodID, void* result_buffer,
                                       Args... args) const {
  if constexpr (sizeof...(Args) > 0) {
    static_assert(
        !((std::is_const_v<Args> || std::is_volatile_v<Args>) || ...));
    constexpr std::array argTypeIDs = {TypeID_of<Args>...};
    std::array args_buffer{reinterpret_cast<std::size_t>(&args)...};
    return Invoke(methodID, result_buffer, Span<const TypeID>{argTypeIDs},
                  static_cast<void*>(args_buffer.data()));
  } else
    return Invoke(methodID, result_buffer);
}

template <typename T, typename... Args>
T ObjectPtrBase::Invoke(StrID methodID, Args... args) const {
  if constexpr (sizeof...(Args) > 0) {
    static_assert(
        !((std::is_const_v<Args> || std::is_volatile_v<Args>) || ...));
    constexpr std::array argTypeIDs = {TypeID_of<Args>...};
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
    constexpr std::array argTypeIDs = {TypeID_of<Args>...};
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

OBJECT_PTR_DEFINE_OPERATOR_T(ObjectPtrBase, +, add)
OBJECT_PTR_DEFINE_OPERATOR_T(ObjectPtrBase, -, sub)
OBJECT_PTR_DEFINE_OPERATOR_T(ObjectPtrBase, *, mul)
OBJECT_PTR_DEFINE_OPERATOR_T(ObjectPtrBase, /, div)
OBJECT_PTR_DEFINE_OPERATOR_T(ObjectPtrBase, %, mod)
OBJECT_PTR_DEFINE_OPERATOR_T(ObjectPtrBase, &, band)
OBJECT_PTR_DEFINE_OPERATOR_T(ObjectPtrBase, |, bor)
OBJECT_PTR_DEFINE_OPERATOR_T(ObjectPtrBase, ^, bxor)

OBJECT_PTR_DEFINE_CONTAINER_T(ObjectPtrBase, count)

//
// ConstObjectPtr
///////////////////

OBJECT_PTR_DEFINE_OPERATOR_T(ConstObjectPtr, ->*, member_of_pointer)

template <typename... Args>
SharedObject ConstObjectPtr::operator()(Args... args) const {
  return DMInvoke<Args...>(StrIDRegistry::MetaID::operator_call,
                           std::forward<Args>(args)...);
}

OBJECT_PTR_DEFINE_CONTAINER_T(ConstObjectPtr, at)
OBJECT_PTR_DEFINE_CONTAINER_T(ConstObjectPtr, find)
OBJECT_PTR_DEFINE_CONTAINER_T(ConstObjectPtr, lower_bound)
OBJECT_PTR_DEFINE_CONTAINER_T(ConstObjectPtr, upper_bound)
OBJECT_PTR_DEFINE_CONTAINER_T(ConstObjectPtr, equal_range)

//
// ObjectPtr
//////////////

template <typename... Args>
InvocableResult ObjectPtr::IsInvocable(StrID methodID) const {
  constexpr std::array argTypeIDs = {TypeID_of<Args>...};
  return IsInvocable(methodID, Span<const TypeID>{argTypeIDs});
}

template <typename T>
T ObjectPtr::InvokeRet(StrID methodID, Span<const TypeID> argTypeIDs,
                       void* args_buffer) const {
  if constexpr (!std::is_void_v<T>) {
    using U =
        std::conditional_t<std::is_reference_v<T>, std::add_pointer_t<T>, T>;
    std::uint8_t result_buffer[sizeof(U)];
    InvokeResult result =
        Invoke(methodID, result_buffer, argTypeIDs, args_buffer);
    assert(result.resultID == TypeID_of<T>);
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
    constexpr std::array argTypeIDs = {TypeID_of<Args>...};
    std::array args_buffer{reinterpret_cast<std::size_t>(&args)...};
    return Invoke(methodID, result_buffer, Span<const TypeID>{argTypeIDs},
                  static_cast<void*>(args_buffer.data()));
  } else
    return Invoke(methodID, result_buffer);
}

template <typename T, typename... Args>
T ObjectPtr::Invoke(StrID methodID, Args... args) const {
  if constexpr (sizeof...(Args) > 0) {
    static_assert(
        !((std::is_const_v<Args> || std::is_volatile_v<Args>) || ...));
    constexpr std::array argTypeIDs = {TypeID_of<Args>...};
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
    constexpr std::array argTypeIDs = {TypeID_of<Args>...};
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

OBJECT_PTR_DEFINE_OPERATOR_T(ObjectPtr, =, assign)
OBJECT_PTR_DEFINE_OPERATOR_T(ObjectPtr, +=, assign_add)
OBJECT_PTR_DEFINE_OPERATOR_T(ObjectPtr, -=, assign_sub)
OBJECT_PTR_DEFINE_OPERATOR_T(ObjectPtr, *=, assign_mul)
OBJECT_PTR_DEFINE_OPERATOR_T(ObjectPtr, /=, assign_div)
OBJECT_PTR_DEFINE_OPERATOR_T(ObjectPtr, %=, assign_mod)
OBJECT_PTR_DEFINE_OPERATOR_T(ObjectPtr, &=, assign_band)
OBJECT_PTR_DEFINE_OPERATOR_T(ObjectPtr, |=, assign_bor)
OBJECT_PTR_DEFINE_OPERATOR_T(ObjectPtr, ^=, assign_bxor)
OBJECT_PTR_DEFINE_OPERATOR_T(ObjectPtr, <<=, assign_lshift)
OBJECT_PTR_DEFINE_OPERATOR_T(ObjectPtr, >>=, assign_rshift)

OBJECT_PTR_DEFINE_OPERATOR_T(ObjectPtr, [], subscript)
OBJECT_PTR_DEFINE_OPERATOR_T(ObjectPtr, ->*, member_of_pointer)

template <typename... Args>
SharedObject ObjectPtr::operator()(Args... args) const {
  return DMInvoke<Args...>(StrIDRegistry::MetaID::operator_call,
                           std::forward<Args>(args)...);
}

template <typename T>
SharedObject ObjectPtr::operator<<(T&& in) const {
  return ADMInvoke<T>(StrIDRegistry::MetaID::operator_lshift,
                      std::forward<T>(in));
}

//
// container
//////////////

OBJECT_PTR_DEFINE_CONTAINER_VARS_T(ObjectPtr, assign)

// - element access

OBJECT_PTR_DEFINE_CONTAINER_T(ObjectPtr, at)

// - capacity

OBJECT_PTR_DEFINE_CONTAINER_T(ObjectPtr, resize)

// - modifiers

OBJECT_PTR_DEFINE_CONTAINER_VARS_T(ObjectPtr, insert)
OBJECT_PTR_DEFINE_CONTAINER_VARS_T(ObjectPtr, insert_or_assign)
OBJECT_PTR_DEFINE_CONTAINER_T(ObjectPtr, erase)
OBJECT_PTR_DEFINE_CONTAINER_T(ObjectPtr, push_front)
OBJECT_PTR_DEFINE_CONTAINER_T(ObjectPtr, push_back)
OBJECT_PTR_DEFINE_CONTAINER_T(ObjectPtr, swap)
OBJECT_PTR_DEFINE_CONTAINER_T(ObjectPtr, merge)
OBJECT_PTR_DEFINE_CONTAINER_T(ObjectPtr, extract)

// - lookup

OBJECT_PTR_DEFINE_CONTAINER_T(ObjectPtr, find)
OBJECT_PTR_DEFINE_CONTAINER_T(ObjectPtr, lower_bound)
OBJECT_PTR_DEFINE_CONTAINER_T(ObjectPtr, upper_bound)
OBJECT_PTR_DEFINE_CONTAINER_T(ObjectPtr, equal_range)

//
// SharedObjectBase
/////////////////////

SHARED_OBJECT_DEFINE_OPERATOR_T(SharedObjectBase, +)
SHARED_OBJECT_DEFINE_OPERATOR_T(SharedObjectBase, -)
SHARED_OBJECT_DEFINE_OPERATOR_T(SharedObjectBase, *)
SHARED_OBJECT_DEFINE_OPERATOR_T(SharedObjectBase, /)
SHARED_OBJECT_DEFINE_OPERATOR_T(SharedObjectBase, %)
SHARED_OBJECT_DEFINE_OPERATOR_T(SharedObjectBase, &)
SHARED_OBJECT_DEFINE_OPERATOR_T(SharedObjectBase, |)
SHARED_OBJECT_DEFINE_OPERATOR_T(SharedObjectBase, ^)

//
// SharedConstObject
//////////////////////

SHARED_OBJECT_DEFINE_OPERATOR_T(SharedConstObject, [])
SHARED_OBJECT_DEFINE_OPERATOR_T(SharedConstObject, ->*)

template <typename... Args>
SharedObject SharedConstObject::operator()(Args... args) const {
  return AsObjectPtr()->operator()<Args...>(std::forward<Args>(args)...);
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

namespace My::MyDRefl {
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

DEFINE_OPERATOR_LSHIFT(std::ostream, ConstObjectPtr)
DEFINE_OPERATOR_LSHIFT(std::ostream, ObjectPtr)
DEFINE_OPERATOR_LSHIFT(std::ostream, SharedConstObject)
DEFINE_OPERATOR_LSHIFT(std::ostream, SharedObject)
DEFINE_OPERATOR_LSHIFT(std::ostringstream, ConstObjectPtr)
DEFINE_OPERATOR_LSHIFT(std::ostringstream, ObjectPtr)
DEFINE_OPERATOR_LSHIFT(std::ostringstream, SharedConstObject)
DEFINE_OPERATOR_LSHIFT(std::ostringstream, SharedObject)
DEFINE_OPERATOR_LSHIFT(std::ofstream, ConstObjectPtr)
DEFINE_OPERATOR_LSHIFT(std::ofstream, ObjectPtr)
DEFINE_OPERATOR_LSHIFT(std::ofstream, SharedConstObject)
DEFINE_OPERATOR_LSHIFT(std::ofstream, SharedObject)
DEFINE_OPERATOR_LSHIFT(std::iostream, ConstObjectPtr)
DEFINE_OPERATOR_LSHIFT(std::iostream, ObjectPtr)
DEFINE_OPERATOR_LSHIFT(std::iostream, SharedConstObject)
DEFINE_OPERATOR_LSHIFT(std::iostream, SharedObject)
DEFINE_OPERATOR_LSHIFT(std::stringstream, ConstObjectPtr)
DEFINE_OPERATOR_LSHIFT(std::stringstream, ObjectPtr)
DEFINE_OPERATOR_LSHIFT(std::stringstream, SharedConstObject)
DEFINE_OPERATOR_LSHIFT(std::stringstream, SharedObject)
DEFINE_OPERATOR_LSHIFT(std::fstream, ConstObjectPtr)
DEFINE_OPERATOR_LSHIFT(std::fstream, ObjectPtr)
DEFINE_OPERATOR_LSHIFT(std::fstream, SharedConstObject)
DEFINE_OPERATOR_LSHIFT(std::fstream, SharedObject)

DEFINE_OPERATOR_RSHIFT(std::istream, ObjectPtr)
DEFINE_OPERATOR_RSHIFT(std::istream, SharedObject)
DEFINE_OPERATOR_RSHIFT(std::istringstream, ObjectPtr)
DEFINE_OPERATOR_RSHIFT(std::istringstream, SharedObject)
DEFINE_OPERATOR_RSHIFT(std::ifstream, ObjectPtr)
DEFINE_OPERATOR_RSHIFT(std::ifstream, SharedObject)
DEFINE_OPERATOR_RSHIFT(std::iostream, ObjectPtr)
DEFINE_OPERATOR_RSHIFT(std::iostream, SharedObject)
DEFINE_OPERATOR_RSHIFT(std::stringstream, ObjectPtr)
DEFINE_OPERATOR_RSHIFT(std::stringstream, SharedObject)
DEFINE_OPERATOR_RSHIFT(std::fstream, ObjectPtr)
DEFINE_OPERATOR_RSHIFT(std::fstream, SharedObject)
}  // namespace My::MyDRefl

#undef OBJECT_PTR_DEFINE_OPERATOR_T
#undef OBJECT_PTR_DEFINE_CONTAINER_T
#undef OBJECT_PTR_DEFINE_CONTAINER_VARS_T
#undef SHARED_OBJECT_DEFINE_OPERATOR_T
#undef DEFINE_OPERATOR_LSHIFT
#undef DEFINE_OPERATOR_RSHIFT
