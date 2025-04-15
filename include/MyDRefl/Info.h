#pragma once

#include "FieldPtr.h"
#include "MethodPtr.h"

#include <set>

namespace My::MyDRefl {
using Attr = SharedObject;

struct AttrLess {
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

class BaseInfo {
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

struct FieldInfo {
  FieldPtr fieldptr;
  AttrSet attrs;
};

struct MethodInfo {
  MethodPtr methodptr;
  AttrSet attrs;
};

// trivial : https://docs.microsoft.com/en-us/cpp/cpp/trivial-standard-layout-and-pod-types?view=msvc-160
// if the type is trivial, it must contains a copy-ctor for type-convertion, and can't register default ctor, dtor
struct TypeInfo {
  size_t size;
  size_t alignment;
  bool is_polymorphic;
  bool is_trivial;
  std::unordered_map<Name, FieldInfo> fieldinfos;
  std::unordered_multimap<Name, MethodInfo> methodinfos;
  std::unordered_map<Type, BaseInfo> baseinfos;
  AttrSet attrs;
};
}  // namespace My::MyDRefl

template <>
constexpr auto My::type_name<My::MyDRefl::AttrSet>() noexcept {
  return TSTR("My::MyDRefl::AttrSet");
}
