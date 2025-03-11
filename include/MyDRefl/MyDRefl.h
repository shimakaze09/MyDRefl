//
// Created by Admin on 11/03/2025.
//

#pragma once

#include <any>
#include <cassert>
#include <functional>
#include <map>
#include <string>
#include <variant>
#include <vector>

namespace My::MyDRefl {
struct TypeInfo;

class Object {
 public:
  Object(size_t id, void* ptr) : id{id}, ptr{ptr} {}

  Object() : id{static_cast<size_t>(-1)}, ptr{nullptr} {}

  void* Pointer() { return ptr; }

  size_t ID() { return id; }

  // non-static
  template <typename T>
  T& Var(size_t offset) {
    return *reinterpret_cast<T*>(reinterpret_cast<uint8_t*>(ptr) + offset);
  }

 private:
  size_t id;
  void* ptr;
};

struct Attr {
  std::any value;
};

using AttrList = std::map<std::string, Attr, std::less<>>;

struct Field {
  struct NonStaticVar {
    std::function<std::any(Object)> get;
    std::function<void(Object, std::any)> set;

    template <typename T>
    static NonStaticVar Init(size_t offset) {
      return {std::function{[=](Object obj) -> std::any {
                return obj.Var<T>(offset);
              }},
              std::function{[=](Object obj, std::any value) {
                obj.Var<T>(offset) = std::any_cast<T>(value);
              }}};
    }
  };

  struct StaticVar {
    std::any data;

    template <typename T>
    static StaticVar Init(T data) {
      return {std::any{data}};
    }
  };

  struct Func {
    std::any data;

    template <typename T>
    static Func Init(T func) {
      return {std::function{func}};
    }

    template <typename T>
    bool TypeIs() {
      return data.type() == typeid(std::function<T>);
    }

    template <typename Ret, typename... Args>
    Ret Call(Args... args) {
      static_assert(std::is_void_v<Ret> || std::is_constructible_v<Ret>);
      assert(TypeIs<Ret(Args...)>());
      return std::any_cast<std::function<Ret(Args...)>>(data)(
          std::forward<Args>(args)...);
    }
  };

  using Value = std::variant<NonStaticVar, StaticVar, Func>;

  static constexpr const char default_constructor[] = "_default_constructor";
  static constexpr const char copy_constructor[] = "_copy_constructor";
  static constexpr const char move_constructor[] = "_move_constructor";
  static constexpr const char destructor[] = "_destructor";

  Value value;
  AttrList attrs;

  bool operator<(const Field& rhs) const {
    if (value.index() != 2 || rhs.value.index() != 2)
      return false;
    return std::get<Func>(value).data.type().hash_code() <
           std::get<Func>(rhs.value).data.type().hash_code();
  }
};

using FieldList = std::multimap<std::string, Field, std::less<>>;

struct Base {
  TypeInfo* info;
  size_t offset;
  bool isVirtual{false};
};

using BaseList = std::map<std::string, Base, std::less<>>;

struct TypeInfo {
  std::string name;

  size_t size;
  size_t alignment;

  BaseList bases;

  AttrList attrs;
  FieldList fields;

  template <typename Ret, typename... Args>
  Ret Call(std::string_view name, Args... args) {
    static_assert(std::is_void_v<Ret> || std::is_constructible_v<Ret>);

    auto low = fields.lower_bound(name);
    auto up = fields.upper_bound(name);
    for (auto iter = low; iter != up; ++iter) {
      if (auto pFunc = std::get_if<Field::Func>(&low->second.value)) {
        if (pFunc->TypeIs<Ret(Args...)>())
          return pFunc->Call<Ret, Args...>(std::forward<Args>(args)...);
      }
    }

    assert("arguments' types are matching failure with functions" && false);
    if constexpr (!std::is_void_v<Ret>)
      return {};
  }

  Object DefaultConstruct() { return Call<Object>(Field::default_constructor); }

  Object CopyConstruct(Object src) {
    return Call<Object, Object>(Field::copy_constructor, src);
  }

  Object MoveConstruct(Object src) {
    return Call<Object, Object>(Field::move_constructor, src);
  }

  void Destruct(Object p) { return Call<void, Object>(Field::destructor, p); }
};

class TypeInfoMngr {
 public:
  static TypeInfoMngr& Instance() {
    static TypeInfoMngr instance;
    return instance;
  }

  TypeInfo& GetTypeInfo(size_t id) { return id2typeinfo[id]; }

 private:
  std::unordered_map<size_t, TypeInfo> id2typeinfo;

  TypeInfoMngr() = default;
};
}  // namespace My::MyDRefl
