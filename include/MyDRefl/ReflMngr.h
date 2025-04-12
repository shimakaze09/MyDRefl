//
// Created by Admin on 1/04/2025.
//

#pragma once

#include "attrs/ContainerType.h"

#include "TypeInfo.h"

namespace My::MyDRefl {
constexpr TypeID GlobalID = TypeIDRegistry::MetaID::global;

class ReflMngr {
 public:
  static ReflMngr& Instance() noexcept {
    static ReflMngr instance;
    return instance;
  }

  //
  // Data
  /////////
  //
  // <typeinfos> does't contain reference/pointer/const/volatile type
  // enum is a special type (all member is static)
  //

  StrIDRegistry nregistry;
  TypeIDRegistry tregistry;

  std::unordered_map<TypeID, TypeInfo> typeinfos;

  // remove cvref
  TypeInfo* GetTypeInfo(TypeID ID);

  // clear order
  // - field attrs
  // - type attrs
  // - type dynamic shared field
  // - typeinfos
  void Clear() noexcept;

  //
  // Factory
  ////////////

  // field_data can be:
  // - static field: pointer to **non-void** type
  // - member object pointer: pointer to **non-void** type
  // - enumerator
  template <auto field_data>
  FieldPtr GenerateFieldPtr();

  // data can be:
  // 1. member object pointer
  // 2. pointer to **non-void** and **non-function** type
  // 3. functor : Value*(Object*)
  // > - result must be an pointer of **non-void** type
  // 4. enumerator
  template <typename T>
  FieldPtr GenerateFieldPtr(T&& data);

  // if T is bufferable, T will be stored as buffer,
  // else we will use std::make_shared to store it
  // require alignof(T) <= alignof(std::max_align_t)
  template <typename T, typename... Args>
  FieldPtr GenerateDynamicFieldPtr(Args&&... args);

  // if T is bufferable, T will be stored as buffer,
  // else we will use std::alloc_shared to store it
  template <typename T, typename Alloc, typename... Args>
  FieldPtr GenerateDynamicFieldPtrByAlloc(const Alloc& alloc, Args&&... args);

  std::pair<StrID, FieldInfo> GenerateField(std::string_view name,
                                            FieldPtr fieldptr,
                                            AttrSet attrs = {}) {
    return {nregistry.Register(name), {std::move(fieldptr), std::move(attrs)}};
  }

  // field_data can be:
  // - static field: pointer to **non-void** type
  // - member object pointer: pointer to **non-void** type
  // - enumerator
  template <auto field_data>
  std::pair<StrID, FieldInfo> GenerateField(std::string_view name,
                                            AttrSet attrs = {}) {
    return GenerateField(name, GenerateFieldPtr<field_data>(),
                         std::move(attrs));
  }

  // data can be:
  // 1. member object pointer
  // 2. pointer to **non-void** and **non-function** type
  // 3. functor : Value*(Object*)
  // > - result must be an pointer of **non-void** type
  // 4. enumerator
  template <
      typename T,
      std::enable_if_t<!std::is_same_v<std::decay_t<T>, FieldPtr>, int> = 0>
  std::pair<StrID, FieldInfo> GenerateField(std::string_view name, T&& data,
                                            AttrSet attrs = {}) {
    return GenerateField(name, GenerateFieldPtr(std::forward<T>(data)),
                         std::move(attrs));
  }

  // if T is bufferable, T will be stored as buffer,
  // else we will use std::make_shared to store it
  // require alignof(T) <= alignof(std::max_align_t)
  template <typename T, typename... Args>
  std::pair<StrID, FieldInfo> GenerateDynamicFieldWithAttrs(
      std::string_view name, AttrSet attrs, Args&&... args) {
    return GenerateField(
        name, GenerateDynamicFieldPtr<T>(std::forward<Args>(args)...),
        std::move(attrs));
  }

  // if T is bufferable, T will be stored as buffer,
  // else we will use std::make_shared to store it
  // require alignof(T) <= alignof(std::max_align_t)
  template <typename T, typename... Args>
  std::pair<StrID, FieldInfo> GenerateDynamicField(std::string_view name,
                                                   Args&&... args) {
    return GenerateDynamicFieldWithAttrs<T>(name, {},
                                            std::forward<Args>(args)...);
  }

  // if T is bufferable, T will be stored as buffer,
  // else we will use std::make_shared to store it
  template <typename T, typename Alloc, typename... Args>
  std::pair<StrID, FieldInfo> GenerateDynamicFieldByAllocWithAttrs(
      std::string_view name, const Alloc& alloc, AttrSet attrs,
      Args&&... args) {
    return GenerateField(
        name,
        GenerateDynamicFieldPtrByAlloc<T>(alloc, std::forward<Args>(args)...),
        std::move(attrs));
  }

  // if T is bufferable, T will be stored as buffer,
  // else we will use std::make_shared to store it
  template <typename T, typename Alloc, typename... Args>
  std::pair<StrID, FieldInfo> GenerateDynamicFieldByAlloc(std::string_view name,
                                                          const Alloc& alloc,
                                                          Args&&... args) {
    return GenerateDynamicFieldByAllocWithAttrs<T>(name, alloc, {},
                                                   std::forward<Args>(args)...);
  }

  template <typename Return>
  ResultDesc GenerateResultDesc();

  template <typename... Params>
  ParamList GenerateParamList() noexcept(sizeof...(Params) == 0);

  // funcptr can be
  // 1. member method : member function pointer
  // 2. static method : function pointer
  template <auto funcptr>
  MethodPtr GenerateMethodPtr();

  template <typename T, typename... Args>
  MethodPtr GenerateConstructorPtr();

  template <typename T>
  MethodPtr GenerateDestructorPtr();

  // Func: Ret(const? Object&, Args...)
  template <typename Func>
  MethodPtr GenerateMemberMethodPtr(Func&& func);

  // Func: Ret(Args...)
  template <typename Func>
  MethodPtr GenerateStaticMethodPtr(Func&& func);

  std::pair<StrID, MethodInfo> GenerateMethod(std::string_view name,
                                              MethodPtr methodptr,
                                              AttrSet attrs = {}) {
    return {nregistry.Register(name), {std::move(methodptr), std::move(attrs)}};
  }

  // funcptr can be
  // 1. member method : member function pointer
  // 2. static method : function pointer
  template <auto funcptr>
  std::pair<StrID, MethodInfo> GenerateMethod(std::string_view name,
                                              AttrSet attrs = {}) {
    return GenerateMethod(name, GenerateMethodPtr<funcptr>(), std::move(attrs));
  }

  // Func: Ret(const? volatile? Object&, Args...)
  template <typename Func>
  std::pair<StrID, MethodInfo> GenerateMemberMethod(std::string_view name,
                                                    Func&& func,
                                                    AttrSet attrs = {}) {
    return GenerateMethod(name, GenerateMemberMethod(std::forward<Func>(func)),
                          std::move(attrs));
  }

  // Func: Ret(Args...)
  template <typename Func>
  std::pair<StrID, MethodInfo> GenerateStaticMethod(std::string_view name,
                                                    Func&& func,
                                                    AttrSet attrs = {}) {
    return GenerateMethod(name, GenerateStaticMethod(std::forward<Func>(func)),
                          std::move(attrs));
  }

  template <typename T, typename... Args>
  std::pair<StrID, MethodInfo> GenerateConstructor(AttrSet attrs = {}) {
    return {StrIDRegistry::MetaID::ctor,
            {GenerateConstructorPtr<T, Args...>(), std::move(attrs)}};
  }

  template <typename T>
  std::pair<StrID, MethodInfo> GenerateDestructor(AttrSet attrs = {}) {
    return {StrIDRegistry::MetaID::dtor,
            {GenerateDestructorPtr<T>(), std::move(attrs)}};
  }

  template <typename Derived, typename Base>
  static BaseInfo GenerateBaseInfo();

  //
  // Modifier
  /////////////

  TypeID RegisterType(std::string_view name, size_t size, size_t alignment);
  StrID AddField(TypeID typeID, std::string_view name, FieldInfo fieldinfo);
  StrID AddMethod(TypeID typeID, std::string_view name, MethodInfo methodinfo);
  bool AddBase(TypeID derivedID, TypeID baseID, BaseInfo baseinfo);
  bool AddAttr(TypeID typeID, const Attr& attr);

  // -- template --

  // RegisterType(type_name<T>(), sizeof(T), alignof(T))
  // AddConstructor<T>()
  // AddConstructor<T, const T&>()
  // AddConstructor<T, T&&>()
  // AddDestructor<T>()
  template <typename T>
  void RegisterType();

  // get TypeID from field_data
  // field_data can be
  // 1. member object pointer
  // 2. enumerator
  template <auto field_data>
  StrID AddField(std::string_view name, AttrSet attrs = {});

  // data can be:
  // 1. member object pointer
  // 2. enumerator
  // 3. pointer to **non-void** and **non-function** type
  // 4. functor : Value*(Object*)
  // > - result must be an pointer of **non-void** type
  template <
      typename T,
      std::enable_if_t<!std::is_same_v<std::decay_t<T>, FieldInfo>, int> = 0>
  StrID AddField(TypeID typeID, std::string_view name, T&& data,
                 AttrSet attrs = {}) {
    return AddField(
        typeID, name,
        {GenerateFieldPtr(std::forward<T>(data)), std::move(attrs)});
  }

  // data can be:
  // 1. member object pointer
  // 2. functor : Value*(Object*)
  // > - result must be an pointer of **non-void** type
  // 3. enumerator
  template <
      typename T,
      std::enable_if_t<!std::is_same_v<std::decay_t<T>, FieldInfo>, int> = 0>
  StrID AddField(std::string_view name, T&& data, AttrSet attrs = {});

  template <typename T, typename... Args>
  StrID AddDynamicFieldWithAttr(TypeID typeID, std::string_view name,
                                AttrSet attrs, Args&&... args) {
    return AddField(typeID, name,
                    {GenerateDynamicFieldPtr<T>(std::forward<Args>(args)...),
                     std::move(attrs)});
  }

  template <typename T, typename... Args>
  StrID AddDynamicField(TypeID typeID, std::string_view name, Args&&... args) {
    return AddDynamicFieldWithAttr<T>(typeID, name, {},
                                      std::forward<Args>(args)...);
  }

  template <typename T, typename Alloc, typename... Args>
  StrID AddDynamicFieldByAllocWithAttr(TypeID typeID, std::string_view name,
                                       const Alloc& alloc, AttrSet attrs,
                                       Args&&... args) {
    return AddField(
        typeID, name,
        {GenerateDynamicFieldPtrByAlloc<T>(alloc, std::forward<Args>(args)...),
         std::move(attrs)});
  }

  template <typename T, typename Alloc, typename... Args>
  StrID AddDynamicFieldByAlloc(TypeID typeID, std::string_view name,
                               const Alloc& alloc, Args&&... args) {
    return AddDynamicFieldByAllocWithAttr<T>(typeID, name, alloc, {},
                                             std::forward<Args>(args)...);
  }

  // funcptr is member function pointer
  // get TypeID from funcptr
  template <auto member_func_ptr>
  StrID AddMethod(std::string_view name, AttrSet attrs = {});

  // funcptr is function pointer
  template <auto func_ptr>
  StrID AddMethod(TypeID typeID, std::string_view name, AttrSet attrs = {});

  template <typename T, typename... Args>
  bool AddConstructor(AttrSet attrs = {});
  template <typename T>
  bool AddDestructor(AttrSet attrs = {});

  // Func: Ret(const? volatile? Object&, Args...)
  template <typename Func>
  StrID AddMemberMethod(std::string_view name, Func&& func, AttrSet attrs = {});

  // Func: Ret(Args...)
  template <typename Func>
  StrID AddStaticMethod(TypeID typeID, std::string_view name, Func&& func,
                        AttrSet attrs = {}) {
    return AddMethod(
        typeID, name,
        {GenerateStaticMethodPtr(std::forward<Func>(func)), std::move(attrs)});
  }

  template <typename Derived, typename... Bases>
  bool AddBases();

  //
  // Cast
  /////////

  ObjectView StaticCast_DerivedToBase(ObjectView obj, TypeID typeID) const;
  ObjectView StaticCast_BaseToDerived(ObjectView obj, TypeID typeID) const;
  ObjectView DynamicCast_BaseToDerived(ObjectView obj, TypeID typeID) const;
  ObjectView StaticCast(ObjectView obj, TypeID typeID) const;
  ObjectView DynamicCast(ObjectView obj, TypeID typeID) const;

  //
  // Field
  //////////
  //
  // - result type of Var maintains the ConstReferenceMode of the input
  //

  // object
  ObjectView Var(TypeID typeID, StrID fieldID);
  // all
  ObjectView Var(ObjectView obj, StrID fieldID);
  // all, for diamond inheritance
  ObjectView Var(ObjectView obj, TypeID baseID, StrID fieldID);

  //
  // Invoke
  ///////////
  //
  // - auto search methods in bases
  // - support overload
  // - require IsCompatible()
  //

  // parameter <- argument
  // - same
  // - reference
  // > - 0 (invalid), 1 (convertible), 2 (copy)
  // > - table
  //     |    -     | T | T & | const T & | T&& | const T&& |
  //     |      T   | - |  2  |     2     |  1  |     2     |
  //     |      T & | 0 |  -  |     0     |  0  |     0     |
  //     |const T & | 1 |  1  |     -     |  1  |     1     |
  //     |      T&& | 1 |  0  |     0     |  -  |     0     |
  //     |const T&& | 1 |  0  |     0     |  1  |     -     |
  // - direct constructible
  bool IsCompatible(std::span<const TypeID> paramTypeIDs,
                    std::span<const TypeID> argTypeIDs) const;

  InvocableResult IsStaticInvocable(
      TypeID typeID, StrID methodID,
      std::span<const TypeID> argTypeIDs = {}) const;
  InvocableResult IsConstInvocable(
      TypeID typeID, StrID methodID,
      std::span<const TypeID> argTypeIDs = {}) const;
  InvocableResult IsInvocable(TypeID typeID, StrID methodID,
                              std::span<const TypeID> argTypeIDs = {}) const;

  InvokeResult Invoke(TypeID typeID, StrID methodID,
                      void* result_buffer = nullptr,
                      std::span<const TypeID> argTypeIDs = {},
                      ArgPtrBuffer argptr_buffer = nullptr) const;

  InvokeResult Invoke(ObjectView obj, StrID methodID,
                      void* result_buffer = nullptr,
                      std::span<const TypeID> argTypeIDs = {},
                      ArgPtrBuffer argptr_buffer = nullptr) const;

  // -- template --

  template <typename... Args>
  InvocableResult IsStaticInvocable(TypeID typeID, StrID methodID) const;
  template <typename... Args>
  InvocableResult IsConstInvocable(TypeID typeID, StrID methodID) const;
  template <typename... Args>
  InvocableResult IsInvocable(TypeID typeID, StrID methodID) const;

  template <typename T>
  T InvokeRet(TypeID typeID, StrID methodID,
              std::span<const TypeID> argTypeIDs = {},
              ArgPtrBuffer argptr_buffer = nullptr) const;
  template <typename T>
  T InvokeRet(ObjectView obj, StrID methodID,
              std::span<const TypeID> argTypeIDs = {},
              ArgPtrBuffer argptr_buffer = nullptr) const;

  template <typename... Args>
  InvokeResult InvokeArgs(TypeID typeID, StrID methodID, void* result_buffer,
                          Args&&... args) const;
  template <typename... Args>
  InvokeResult InvokeArgs(ObjectView obj, StrID methodID, void* result_buffer,
                          Args&&... args) const;

  template <typename T, typename... Args>
  T Invoke(TypeID typeID, StrID methodID, Args&&... args) const;
  template <typename T, typename... Args>
  T Invoke(ObjectView obj, StrID methodID, Args&&... args) const;

  //
  // Meta
  /////////

  bool IsNonArgConstructible(TypeID typeID,
                             std::span<const TypeID> argTypeIDs) const;
  bool IsConstructible(TypeID typeID, std::span<const TypeID> argTypeIDs) const;
  bool IsDestructible(TypeID typeID) const;
  bool IsCopyConstructible(TypeID typeID) const;
  bool IsMoveConstructible(TypeID typeID) const;

  bool NonArgConstruct(ObjectView obj, std::span<const TypeID> argTypeIDs,
                       ArgPtrBuffer argptr_buffer) const;
  bool Construct(ObjectView obj, std::span<const TypeID> argTypeIDs,
                 ArgPtrBuffer argptr_buffer) const;
  bool Destruct(ObjectView obj) const;

  void* Malloc(size_t size) const;
  bool Free(void* ptr) const;

  void* AlignedMalloc(size_t size, size_t alignment) const;
  bool AlignedFree(void* ptr) const;

  ObjectView NonArgCopyNew(TypeID typeID, std::span<const TypeID> argTypeIDs,
                           ArgPtrBuffer argptr_buffer) const;
  ObjectView New(TypeID typeID, std::span<const TypeID> argTypeIDs,
                 ArgPtrBuffer argptr_buffer) const;
  bool Delete(ObjectView obj) const;

  SharedObject MakeShared(TypeID typeID, std::span<const TypeID> argTypeIDs,
                          ArgPtrBuffer argptr_buffer) const;

  // -- template --

  template <typename... Args>
  bool IsConstructible(TypeID typeID) const;

  template <typename... Args>
  bool Construct(ObjectView obj, Args&&... args) const;

  template <typename... Args>
  ObjectView New(TypeID typeID, Args&&... args) const;

  template <typename... Args>
  SharedObject MakeShared(TypeID typeID, Args&&... args) const;

  // - if T is not register, call RegisterType<T>()
  // - call AddConstructor<T, Args...>()
  template <typename T, typename... Args>
  ObjectView NewAuto(Args... args);

  // - if T is not register, call RegisterType<T>()
  // - call AddConstructor<T, Args...>()
  template <typename T, typename... Args>
  SharedObject MakeSharedAuto(Args... args);

  //
  // Algorithm
  //////////////

  // ForEach (DFS)

  // self typeID and all bases' typeID
  void ForEachTypeID(TypeID typeID,
                     const std::function<bool(TypeID)>& func) const;

  // self type and all bases' type
  void ForEachType(TypeID typeID,
                   const std::function<bool(TypeRef)>& func) const;

  // self fields and all bases' fields
  void ForEachField(TypeID typeID,
                    const std::function<bool(TypeRef, FieldRef)>& func) const;

  // self methods and all bases' methods
  void ForEachMethod(TypeID typeID,
                     const std::function<bool(TypeRef, MethodRef)>& func) const;

  // self object vars and all bases' object vars
  void ForEachVar(
      TypeID typeID,
      const std::function<bool(TypeRef, FieldRef, ObjectView)>& func) const;

  // self vars and all bases' vars
  void ForEachVar(
      ObjectView obj,
      const std::function<bool(TypeRef, FieldRef, ObjectView)>& func) const;

  // self owned vars and all bases' owned vars
  void ForEachOwnedVar(
      ObjectView obj,
      const std::function<bool(TypeRef, FieldRef, ObjectView)>& func) const;

  // Gather (DFS)

  std::vector<TypeID> GetTypeIDs(TypeID typeID);
  std::vector<TypeRef> GetTypes(TypeID typeID);
  std::vector<TypeFieldRef> GetTypeFields(TypeID typeID);
  std::vector<FieldRef> GetFields(TypeID typeID);
  std::vector<TypeMethodRef> GetTypeMethods(TypeID typeID);
  std::vector<MethodRef> GetMethods(TypeID typeID);
  std::vector<std::tuple<TypeRef, FieldRef, ObjectView>> GetTypeFieldVars(
      TypeID typeID);
  std::vector<ObjectView> GetVars(TypeID typeID);
  std::vector<std::tuple<TypeRef, FieldRef, ObjectView>> GetTypeFieldVars(
      ObjectView obj);
  std::vector<ObjectView> GetVars(ObjectView obj);
  std::vector<std::tuple<TypeRef, FieldRef, ObjectView>> GetTypeFieldOwnedVars(
      ObjectView obj);
  std::vector<ObjectView> GetOwnedVars(ObjectView obj);

  // Find (DFS)

  std::optional<TypeID> FindTypeID(
      TypeID typeID, const std::function<bool(TypeID)>& func) const;
  std::optional<TypeRef> FindType(
      TypeID typeID, const std::function<bool(TypeRef)>& func) const;
  std::optional<FieldRef> FindField(
      TypeID typeID, const std::function<bool(FieldRef)>& func) const;
  std::optional<MethodRef> FindMethod(
      TypeID typeID, const std::function<bool(MethodRef)>& func) const;
  ObjectView FindVar(TypeID typeID,
                     const std::function<bool(ObjectView)>& func) const;
  ObjectView FindVar(ObjectView obj,
                     const std::function<bool(ObjectView)>& func) const;
  ObjectView FindOwnedVar(ObjectView obj,
                          const std::function<bool(ObjectView)>& func) const;

  // Contains (DFS)

  bool ContainsBase(TypeID typeID, TypeID baseID) const;
  bool ContainsField(TypeID typeID, StrID fieldID) const;
  bool ContainsMethod(TypeID typeID, StrID methodID) const;
  bool ContainsVariableMethod(TypeID typeID, StrID methodID) const;
  bool ContainsConstMethod(TypeID typeID, StrID methodID) const;
  bool ContainsStaticMethod(TypeID typeID, StrID methodID) const;

  //
  // Memory
  ///////////
  //
  // - MInvoke will allocate buffer for result, and move to SharedObject
  // - if result is reference, SharedObject's Ptr is a pointer of referenced object
  // - DMInvoke's 'D' means 'default' (use the default memory resource)
  //

  std::pmr::synchronized_pool_resource* GetTemporaryResource() const {
    return &temporary_resource;
  }

  SharedObject MInvoke(TypeID typeID, StrID methodID,
                       std::span<const TypeID> argTypeIDs = {},
                       ArgPtrBuffer argptr_buffer = nullptr,
                       std::pmr::memory_resource* result_rsrc =
                           std::pmr::get_default_resource()) const;

  SharedObject MInvoke(ObjectView obj, StrID methodID,
                       std::span<const TypeID> argTypeIDs = {},
                       ArgPtrBuffer argptr_buffer = nullptr,
                       std::pmr::memory_resource* result_rsrc =
                           std::pmr::get_default_resource()) const;

  template <typename... Args>
  SharedObject MInvoke(TypeID typeID, StrID methodID,
                       std::pmr::memory_resource* result_rsrc,
                       Args&&... args) const;

  template <typename... Args>
  SharedObject MInvoke(ObjectView obj, StrID methodID,
                       std::pmr::memory_resource* result_rsrc,
                       Args&&... args) const;

  template <typename... Args>
  SharedObject DMInvoke(TypeID typeID, StrID methodID, Args&&... args) const;

  template <typename... Args>
  SharedObject DMInvoke(ObjectView obj, StrID methodID, Args&&... args) const;

  ObjectView NonArgCopyMNew(TypeID typeID, std::pmr::memory_resource* rsrc,
                            std::span<const TypeID> argTypeIDs,
                            ArgPtrBuffer argptr_buffer) const;
  ObjectView MNew(TypeID typeID, std::pmr::memory_resource* rsrc,
                  std::span<const TypeID> argTypeIDs,
                  ArgPtrBuffer argptr_buffer) const;
  bool MDelete(ObjectView obj, std::pmr::memory_resource* rsrc) const;

  template <typename... Args>
  ObjectView MNew(TypeID typeID, std::pmr::memory_resource* rsrc,
                  Args&&... args) const;

  //
  // Type
  /////////
  //
  // - 'reference' include lvalue reference and rvalue reference
  //

  bool IsConst(TypeID ID) const;
  // const{T}, &/&&{const{T}}
  bool IsReadOnly(TypeID ID) const;
  bool IsReference(TypeID ID) const;
  ConstReferenceMode GetConstReferenceMode(TypeID ID) const;

  TypeID RemoveConst(TypeID ID) const;
  TypeID RemoveReference(TypeID ID) const;
  TypeID RemoveConstReference(TypeID ID) const;

  TypeID AddConst(TypeID ID);
  TypeID AddLValueReference(TypeID ID);
  TypeID AddLValueReferenceWeak(TypeID ID);
  TypeID AddRValueReference(TypeID ID);
  TypeID AddConstLValueReference(TypeID ID);
  TypeID AddConstRValueReference(TypeID ID);

 private:
  ReflMngr();
  ~ReflMngr();

  // for
  // - argument copy
  // - user argument buffer
  mutable std::pmr::synchronized_pool_resource temporary_resource;
};

inline static ReflMngr& Mngr = ReflMngr::Instance();
}  // namespace My::MyDRefl

#include "details/ReflMngr.inl"
