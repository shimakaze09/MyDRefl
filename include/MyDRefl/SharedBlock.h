//
// Created by Admin on 2/04/2025.
//

#pragma once

#include "ObjectPtr.h"

#include <memory>

namespace My::MyDRefl {
class SharedBlock {
 public:
  //
  // Constructor
  ////////////////

  constexpr SharedBlock() noexcept = default;

  constexpr SharedBlock(std::nullptr_t) noexcept {}

  SharedBlock(void* ptr, void (*deleter)(void*)) : ptr{ptr, deleter} {}

  SharedBlock(void (*deleter)(void*)) : ptr{nullptr, deleter} {}

  explicit SharedBlock(const std::weak_ptr<void>& ptr) : ptr{ptr} {}

  SharedBlock(const std::shared_ptr<void>& ptr) noexcept : ptr{ptr} {}

  SharedBlock(std::shared_ptr<void>&& ptr) noexcept : ptr{std::move(ptr)} {}

  SharedBlock(SharedBlock&& attr) noexcept : ptr{std::move(attr.ptr)} {}

  SharedBlock(const SharedBlock& attr) : ptr{attr.ptr} {}  // only exception

  template <typename U>
  explicit SharedBlock(U* ptr) : ptr{std::shared_ptr<void>{ptr}} {}

  template <typename U, typename Deleter>
  SharedBlock(U* ptr, Deleter d) : ptr{ptr, std::move(d)} {}

  template <typename Deleter>
  SharedBlock(std::nullptr_t ptr, Deleter d) : ptr{ptr, std::move(d)} {}

  template <typename U, typename Deleter, typename Alloc>
  SharedBlock(U* ptr, Deleter d, Alloc alloc)
      : ptr{ptr, std::move(d), std::move(alloc)} {}

  template <typename Deleter, typename Alloc>
  SharedBlock(std::nullptr_t ptr, Deleter d, Alloc alloc)
      : ptr{ptr, std::move(d), std::move(alloc)} {}

  template <typename U>
  explicit SharedBlock(const std::weak_ptr<U>& ptr) : ptr{ptr} {}

  template <typename Y, typename Deleter>
  SharedBlock(std::unique_ptr<Y, Deleter>&& r) : ptr{std::move(r)} {}

  template <typename U>
  SharedBlock(const std::shared_ptr<U>& ptr) noexcept : ptr{ptr} {}

  template <typename U>
  SharedBlock(std::shared_ptr<U>&& ptr) noexcept : ptr{std::move(ptr)} {}

  //
  // Assign
  ///////////

  SharedBlock& operator=(SharedBlock& rhs) noexcept {
    ptr = rhs.ptr;
    return *this;
  }

  SharedBlock& operator=(SharedBlock&& rhs) noexcept {
    ptr = std::move(rhs.ptr);
    return *this;
  }

  SharedBlock& operator=(const std::shared_ptr<void>& rhs) noexcept {
    ptr = rhs;
    return *this;
  }

  SharedBlock& operator=(std::shared_ptr<void>&& rhs) noexcept {
    ptr = std::move(rhs);
    return *this;
  }

  SharedBlock& operator=(std::nullptr_t) noexcept {
    reset();
    return *this;
  }

  template <typename U>
  SharedBlock& operator=(const std::shared_ptr<U>& rhs) noexcept {
    ptr = rhs;
    return *this;
  }

  template <typename U>
  SharedBlock& operator=(std::shared_ptr<U>&& rhs) noexcept {
    ptr = std::move(rhs);
    return *this;
  }

  template <typename U, typename Deleter>
  SharedBlock& operator=(std::unique_ptr<U, Deleter>&& rhs) {
    ptr = std::move(rhs);
    return *this;
  }

  //
  // Cast
  /////////

  std::shared_ptr<void>& cast_to_shared_ptr() & noexcept { return ptr; }

  std::shared_ptr<void> cast_to_shared_ptr() && noexcept {
    return std::move(ptr);
  }

  std::shared_ptr<const void> cast_to_shared_ptr() const& noexcept {
    return std::static_pointer_cast<const void>(ptr);
  }

  operator std::shared_ptr<void>&() & noexcept { return ptr; }

  operator std::shared_ptr<void>() && noexcept { return std::move(ptr); }

  operator std::shared_ptr<const void>() const& noexcept { return ptr; }

  //
  // Modifiers
  //////////////

  void reset() noexcept { ptr.reset(); }

  template <typename U>
  void reset(U* ptrU) {
    ptr.reset(ptrU);
  }

  template <typename U, typename Deleter>
  void reset(U* ptrU, Deleter d) {
    ptr.reset(ptrU, std::move(d));
  }

  template <typename U, typename Deleter, typename Alloc>
  void reset(U* ptrU, Deleter d, Alloc alloc) {
    ptr.reset(ptrU, std::move(d), std::move(alloc));
  }

  void swap(SharedBlock& rhs) noexcept { return ptr.swap(rhs.ptr); }

  void swap(std::shared_ptr<void>& rhs) noexcept { return ptr.swap(rhs); }

  //
  // Observers
  //////////////

  void* get() noexcept { return ptr.get(); }

  const void* get() const noexcept { return ptr.get(); }

  template <typename T>
  T* as_ptr() noexcept {
    return reinterpret_cast<T*>(get());
  }

  template <typename T>
  const T* as_ptr() const noexcept {
    return const_cast<SharedBlock*>(this)->as_ptr<T>();
  }

  ObjectPtr as_object(TypeID id) noexcept { return {id, get()}; }

  ConstObjectPtr as_object(TypeID id) const noexcept { return {id, get()}; }

  template <typename T>
  T& as() noexcept {
    assert(get());
    return *as_ptr<T>();
  }

  template <typename T>
  const T& as() const noexcept {
    assert(get());
    return *as_ptr<T>();
  }

  long use_count() const noexcept { return ptr.use_count(); }

  explicit operator bool() const noexcept { return static_cast<bool>(ptr); }

 private:
  std::shared_ptr<void> ptr;
};

template <typename T, class... Args>
SharedBlock MakeSharedBlock(Args&&... args) {
  if constexpr (std::is_aggregate_v<T> && !std::is_constructible_v<T, Args...>)
    return SharedBlock{new T{std::forward<Args>(args)...}};
  else
    return {std::make_shared<T>(std::forward<Args>(args)...)};
}

template <typename T, class Alloc, class... Args>
SharedBlock AllocateSharedBlock(const Alloc& alloc, Args&&... args) {
  return {std::allocate_shared<T>(alloc, std::forward<Args>(args)...)};
}
}  // namespace My::MyDRefl

template <>
struct std::hash<Ubpa::UDRefl::SharedBlock> {
  std::size_t operator()(const Ubpa::UDRefl::SharedBlock& obj) noexcept {
    return std::hash<const void*>()(obj.get());
  }
};

inline bool operator==(const Ubpa::UDRefl::SharedBlock& left,
                       const Ubpa::UDRefl::SharedBlock& right) noexcept {
  return left.get() == right.get();
}

inline bool operator!=(const Ubpa::UDRefl::SharedBlock& left,
                       const Ubpa::UDRefl::SharedBlock& right) noexcept {
  return left.get() != right.get();
}

inline bool operator<(const Ubpa::UDRefl::SharedBlock& left,
                      const Ubpa::UDRefl::SharedBlock& right) noexcept {
  return left.get() < right.get();
}

inline bool operator>=(const Ubpa::UDRefl::SharedBlock& left,
                       const Ubpa::UDRefl::SharedBlock& right) noexcept {
  return left.get() >= right.get();
}

inline bool operator>(const Ubpa::UDRefl::SharedBlock& left,
                      const Ubpa::UDRefl::SharedBlock& right) noexcept {
  return left.get() > right.get();
}

inline bool operator<=(const Ubpa::UDRefl::SharedBlock& left,
                       const Ubpa::UDRefl::SharedBlock& right) noexcept {
  return left.get() <= right.get();
}

template <class Elem, typename Traits>
std::basic_ostream<Elem, Traits>& operator<<(
    std::basic_ostream<Elem, Traits>& out,
    const Ubpa::UDRefl::SharedBlock& obj) {
  return out << obj.get();
}

namespace std {
inline void swap(Ubpa::UDRefl::SharedBlock& left,
                 Ubpa::UDRefl::SharedBlock& right) noexcept {
  left.swap(right);
}

inline void swap(Ubpa::UDRefl::SharedBlock& left,
                 shared_ptr<void>& right) noexcept {
  left.swap(right);
}

inline void swap(shared_ptr<void>& left,
                 Ubpa::UDRefl::SharedBlock& right) noexcept {
  right.swap(left);
}
}  // namespace std
