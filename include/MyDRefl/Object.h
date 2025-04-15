#pragma once

#include "Basic.h"
#include "IDRegistry.h"

//#include <span>

namespace My::MyDRefl {
// pointer const array type (pointer is const, and pointer to non - const / referenced object)
using ArgPtrBuffer = void* const*;

class ArgsView {
 public:
  constexpr ArgsView() noexcept : buffer{nullptr} {}

  constexpr ArgsView(ArgPtrBuffer buffer,
                     std::span<const Type> argTypes) noexcept
      : buffer{buffer}, argTypes{argTypes} {}

  constexpr ArgPtrBuffer Buffer() const noexcept { return buffer; }

  constexpr std::span<const Type> Types() const noexcept { return argTypes; }

  constexpr ObjectView operator[](size_t idx) const noexcept;

 private:
  ArgPtrBuffer buffer;
  std::span<const Type> argTypes;
};

class ObjectView {
 public:
  constexpr ObjectView() noexcept : ptr{nullptr} {}

  constexpr ObjectView(Type type, void* ptr) noexcept : type{type}, ptr{ptr} {}

  explicit constexpr ObjectView(Type type) noexcept
      : ObjectView{type, nullptr} {}

  template <typename T>
  requires std::negation_v<std::is_reference<T>>&&
      std::negation_v<std::is_same<std::remove_cvref_t<T>, Type>>&&
          NonObjectAndView<T> constexpr explicit ObjectView(T& obj) noexcept
      : ObjectView{Type_of<T>,
                   const_cast<void*>(static_cast<const void*>(&obj))} {}

  constexpr const Type& GetType() const noexcept { return type; }

  constexpr void* const& GetPtr() const noexcept { return ptr; }

  explicit operator bool() const noexcept;

  template <typename T>
  constexpr auto* AsPtr() const noexcept;

  template <typename T>
  constexpr decltype(auto) As() const noexcept;

  //////////////
  // ReflMngr //
  //////////////

  //
  // Cast
  /////////

  ObjectView StaticCast_DerivedToBase(Type base) const;
  ObjectView StaticCast_BaseToDerived(Type derived) const;
  ObjectView DynamicCast_BaseToDerived(Type derived) const;
  ObjectView StaticCast(Type type) const;
  ObjectView DynamicCast(Type type) const;

  //
  // Invoke
  ///////////

  Type IsInvocable(Name method_name, std::span<const Type> argTypes = {},
                   MethodFlag flag = MethodFlag::All) const;

  Type BInvoke(Name method_name, void* result_buffer = nullptr,
               ArgsView args = {}, MethodFlag flag = MethodFlag::All) const;

  SharedObject MInvoke(Name method_name, std::pmr::memory_resource* rst_rsrc,
                       std::pmr::memory_resource* temp_args_rsrc,
                       ArgsView args = {},
                       MethodFlag flag = MethodFlag::All) const;

  SharedObject Invoke(Name method_name, ArgsView args = {},
                      MethodFlag flag = MethodFlag::All) const;

  // -- template --

  template <typename... Args>
  Type IsInvocable(Name method_name, MethodFlag flag = MethodFlag::All) const;

  template <typename T>
  T BInvokeRet(Name method_name, ArgsView args = {},
               MethodFlag flag = MethodFlag::All) const;

  template <typename T, typename... Args>
  T BInvoke(Name method_name, MethodFlag flag, Args&&... args) const;

  template <typename... Args>
  SharedObject MInvoke(Name method_name, std::pmr::memory_resource* rst_rsrc,
                       std::pmr::memory_resource* temp_args_rsrc,
                       MethodFlag flag, Args&&... args) const;

  template <typename... Args>
  SharedObject Invoke(Name method_name, Args&&... args) const;

  template <typename T, typename... Args>
  T ABInvoke(Name method_name, MethodFlag flag, Args&&... args) const;

  template <typename... Args>
  SharedObject AMInvoke(Name method_name, std::pmr::memory_resource* rst_rsrc,
                        std::pmr::memory_resource* temp_args_rsrc,
                        MethodFlag flag, Args&&... args) const;

  template <typename... Args>
  SharedObject AInvoke(Name method_name, Args&&... args) const;

  //
  // Var
  ////////

  ObjectView Var(Name field_name, FieldFlag flag = FieldFlag::All) const;
  // for diamond inheritance
  ObjectView Var(Type base, Name field_name,
                 FieldFlag flag = FieldFlag::All) const;

  //
  // Algorithm
  //////////////
  //
  // - only contains APIs with ObjectView in ReflMngr
  //

  void ForEachVar(
      const std::function<bool(InfoTypePair, InfoFieldPair, ObjectView)>& func,
      FieldFlag flag = FieldFlag::All) const;
  std::vector<std::tuple<InfoTypePair, InfoFieldPair, ObjectView>>
  GetTypeFieldVars(FieldFlag flag = FieldFlag::All) const;
  std::vector<ObjectView> GetVars(FieldFlag flag = FieldFlag::All) const;
  ObjectView FindVar(const std::function<bool(ObjectView)>& func,
                     FieldFlag flag = FieldFlag::All) const;

  //
  // Type
  //////////

  ObjectView RemoveConst() const;
  ObjectView RemoveLValueReference() const;
  ObjectView RemoveRValueReference() const;
  ObjectView RemoveReference() const;
  ObjectView RemoveConstReference() const;

  ObjectView AddConst() const;
  ObjectView AddLValueReference() const;
  ObjectView AddLValueReferenceWeak() const;
  ObjectView AddRValueReference() const;
  ObjectView AddConstLValueReference() const;
  ObjectView AddConstRValueReference() const;

  //////////
  // Meta //
  //////////

  //
  // operators
  //////////////

  template <typename T>
  SharedObject operator+(T&& rhs) const;
  template <typename T>
  SharedObject operator-(T&& rhs) const;
  template <typename T>
  SharedObject operator*(T&& rhs) const;
  template <typename T>
  SharedObject operator/(T&& rhs) const;
  template <typename T>
  SharedObject operator%(T&& rhs) const;

  template <typename T>
  SharedObject operator&(T&& rhs) const;
  template <typename T>
  SharedObject operator|(T&& rhs) const;
  template <typename T>
  SharedObject operator^(T&& rhs) const;
  template <typename T>
  SharedObject operator<<(T&& rhs) const;
  template <typename T>
  SharedObject operator>>(T&& rhs) const;

  template <typename T>
  bool operator<(const T& rhs) const;
  template <typename T>
  bool operator<=(const T& rhs) const;
  template <typename T>
  bool operator>(const T& rhs) const;
  template <typename T>
  bool operator>=(const T& rhs) const;

  template <typename Arg>
  requires NonObjectAndView<std::decay_t<Arg>> ObjectView
  operator=(Arg&& rhs) const;

  template <typename T>
  ObjectView operator+=(T&& rhs) const;
  template <typename T>
  ObjectView operator-=(T&& rhs) const;
  template <typename T>
  ObjectView operator*=(T&& rhs) const;
  template <typename T>
  ObjectView operator/=(T&& rhs) const;
  template <typename T>
  ObjectView operator%=(T&& rhs) const;
  template <typename T>
  ObjectView operator&=(T&& rhs) const;
  template <typename T>
  ObjectView operator|=(T&& rhs) const;
  template <typename T>
  ObjectView operator^=(T&& rhs) const;
  template <typename T>
  ObjectView operator<<=(T&& rhs) const;
  template <typename T>
  ObjectView operator>>=(T&& rhs) const;

  SharedObject operator++() const;
  SharedObject operator++(int) const;
  SharedObject operator--() const;
  SharedObject operator--(int) const;
  SharedObject operator+() const;
  SharedObject operator-() const;
  SharedObject operator~() const;
  SharedObject operator*() const;

  template <typename T>
  SharedObject operator[](T&& rhs) const;

  template <typename... Args>
  SharedObject operator()(Args&&... args) const;

  //
  // attrs
  //////////

  ContainerType get_container_type() const;

  //
  // non-member functions
  /////////////////////////

  // tuple & variant

  ObjectView get(std::size_t i) const;
  ObjectView get(Type type) const;

  // - tuple

  std::size_t tuple_size() const;
  Type tuple_element(std::size_t i) const;

  // - variant

  std::size_t variant_size() const;
  Type variant_alternative(std::size_t i) const;
  ObjectView variant_visit_get() const;

  // - iterator

  template <typename T>
  void advance(T&& arg) const;
  template <typename T>
  std::size_t distance(T&& arg) const;
  template <typename T>
  SharedObject next(T&& arg) const;
  template <typename T>
  SharedObject prev(T&& arg) const;
  SharedObject next() const;
  SharedObject prev() const;

  //
  // Container
  //////////////

  template <typename... Args>
  void assign(Args&&... args) const;

  // - iterator

  SharedObject begin() const;
  SharedObject end() const;
  SharedObject cbegin() const;
  SharedObject cend() const;
  SharedObject rbegin() const;
  SharedObject rend() const;
  SharedObject crbegin() const;
  SharedObject crend() const;

  // - capacity

  bool empty() const;
  std::size_t size() const;
  std::size_t capacity() const;
  std::size_t bucket_count() const;
  template <typename... Args>
  void resize(Args&&... args) const;
  void reserve(std::size_t n) const;
  void shrink_to_fit() const;

  // - element access

  template <typename T>
  SharedObject at(T&& rhs) const;
  SharedObject data() const;
  SharedObject front() const;
  SharedObject back() const;
  SharedObject top() const;

  // - modifiers

  void clear() const;
  template <typename... Args>
  SharedObject insert(Args&&... args) const;
  template <typename... Args>
  SharedObject insert_after(Args&&... args) const;
  template <typename... Args>
  SharedObject insert_or_assign(Args&&... args) const;
  template <typename... Args>
  SharedObject erase(Args&&... args) const;
  template <typename... Args>
  SharedObject erase_after(Args&&... args) const;
  template <typename T>
  void push_front(T&& arg) const;
  template <typename T>
  void push_back(T&& arg) const;
  void pop_front() const;
  void pop_back() const;
  template <typename T>
  void push(T&& arg) const;
  void pop() const;
  template <typename T>
  void swap(T&& arg) const;
  template <typename T>
  void merge(T&& arg) const;
  template <typename T>
  SharedObject extract(T&& rhs) const;

  // - list operations

  template <typename... Args>
  void splice_after(Args&&... args) const;
  template <typename... Args>
  void splice(Args&&... args) const;
  template <typename T>
  std::size_t remove(T&& arg) const;
  void reverse() const;
  template <typename T>
  std::size_t unique(T&& arg) const;
  void sort() const;

  // - lookup

  template <typename T>
  std::size_t count(T&& arg) const;
  template <typename T>
  SharedObject find(T&& rhs) const;
  template <typename T>
  SharedObject lower_bound(T&& rhs) const;
  template <typename T>
  SharedObject upper_bound(T&& rhs) const;
  template <typename T>
  SharedObject equal_range(T&& rhs) const;

  // - variant

  std::size_t index() const;
  bool holds_alternative(Type type) const;

  // - optional

  bool has_value() const;
  ObjectView value() const;
  void reset() const;

 protected:
  Type type;
  void* ptr;  // if type is reference, ptr is a pointer of referenced object
};

class SharedObject : public ObjectView {
 public:
  using ObjectView::ObjectView;
  using ObjectView::operator=;

  SharedObject(Type type, SharedBuffer buffer) noexcept
      : ObjectView{type}, buffer{std::move(buffer)} {
    ptr = buffer.get();
  }

  constexpr explicit SharedObject(ObjectView obj) noexcept : ObjectView{obj} {}

  template <typename T>
  SharedObject(Type type, std::shared_ptr<T> buffer) noexcept
      : ObjectView{type, buffer.get()}, buffer{std::move(buffer)} {}

  template <typename Deleter>
  SharedObject(ObjectView obj, Deleter d) noexcept
      : ObjectView{obj}, buffer{obj.GetPtr(), std::move(d)} {}

  template <typename U, typename Deleter, typename Alloc>
  SharedObject(ObjectView obj, Deleter d, Alloc alloc) noexcept
      : ObjectView{obj}, buffer{obj.GetPtr(), std::move(d), alloc} {}

  // set pointer to nullptr
  void Reset() noexcept {
    ptr = nullptr;
    buffer.reset();
  }

  SharedBuffer& GetBuffer() noexcept { return buffer; }

  const SharedBuffer& GetBuffer() const noexcept { return buffer; }

  long UseCount() const noexcept { return buffer.use_count(); }

  bool IsObjectView() const noexcept { return !buffer; }

  void Swap(SharedObject& rhs) noexcept {
    std::swap(type, rhs.type);
    std::swap(ptr, rhs.ptr);
    buffer.swap(rhs.buffer);
  }

 private:
  SharedBuffer buffer;  // if type is reference/void, buffer is empty
};

template <typename T>
constexpr ObjectView ObjectView_of = {Type_of<T>, nullptr};
}  // namespace My::MyDRefl

#include "details/Object.inl"
