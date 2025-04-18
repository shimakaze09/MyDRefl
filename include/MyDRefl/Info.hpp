#pragma once

#include <set>

#include "FieldPtr.hpp"
#include "MethodPtr.hpp"

namespace Smkz::MyDRefl {
using Attr = SharedObject;
struct MyDRefl_core_API AttrLess {
  using is_transparent = int;
  bool operator()(const Attr& lhs, const Attr& rhs) const noexcept {
    return lhs.GetType() < rhs.GetType();
  }
  bool operator()(const Attr& lhs, const Type& rhs) const noexcept {
    return lhs.GetType() < rhs;
  }
  bool operator()(const Type& lhs, const Attr& rhs) const noexcept {
    return lhs < rhs.GetType();
  }
};
using AttrSet = std::set<Attr, AttrLess>;

class MyDRefl_core_API BaseInfo {
 public:
  BaseInfo() noexcept = default;
  BaseInfo(InheritCastFunctions funcs) : funcs{std::move(funcs)} {
    assert(this->funcs.static_derived_to_base);
  }

  bool IsVirtual() const noexcept {
    return !static_cast<bool>(funcs.static_base_to_derived);
  }
  bool IsPolymorphic() const noexcept {
    return static_cast<bool>(funcs.dynamic_base_to_derived);
  }

  void* StaticCast_DerivedToBase(void* ptr) const noexcept {
    return funcs.static_derived_to_base(ptr);
  }
  // require non virtual
  void* StaticCast_BaseToDerived(void* ptr) const noexcept {
    return IsVirtual() ? nullptr : funcs.static_base_to_derived(ptr);
  }
  // require polymorphic
  void* DynamicCast_BaseToDerived(void* ptr) const noexcept {
    return IsPolymorphic() ? funcs.dynamic_base_to_derived(ptr) : nullptr;
  }

 private:
  InheritCastFunctions funcs;
};

struct MyDRefl_core_API FieldInfo {
  FieldPtr fieldptr;
  AttrSet attrs;
};

struct MyDRefl_core_API MethodInfo {
  MethodPtr methodptr;
  AttrSet attrs;
};

// trivial :
// https://docs.microsoft.com/en-us/cpp/cpp/trivial-standard-layout-and-pod-types?view=msvc-160
// if the type is trivial, it must contains a copy-ctor for type-convertion, and
// can't register default ctor, dtor
struct MyDRefl_core_API TypeInfo {
  size_t size;
  size_t alignment;
  bool is_polymorphic;
  bool is_trivial;
  std::unordered_map<Name, FieldInfo> fieldinfos;
  std::unordered_multimap<Name, MethodInfo> methodinfos;
  std::unordered_map<Type, BaseInfo> baseinfos;
  AttrSet attrs;
};
}  // namespace Smkz::MyDRefl

template <>
constexpr auto Smkz::type_name<Smkz::MyDRefl::AttrSet>() noexcept {
  return TSTR("Smkz::MyDRefl::AttrSet");
}
