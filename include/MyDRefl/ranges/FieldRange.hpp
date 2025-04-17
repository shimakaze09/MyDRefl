#pragma once

#include "ObjectTree.hpp"

namespace My::MyDRefl {
// DFS
class MyDRefl_core_API FieldRange {
 public:
  class MyDRefl_core_API iterator {
   public:
    using value_type = std::pair<const Name, FieldInfo>;
    using reference = value_type&;
    using pointer = value_type*;
    using iterator_category = std::forward_iterator_tag;

    iterator(ObjectTree::iterator typeiter, FieldFlag flag = FieldFlag::All);

    iterator& operator++();
    iterator operator++(int);

    reference operator*() const noexcept { return *curfield; }
    pointer operator->() const noexcept { return curfield.operator->(); }

    MyDRefl_core_API friend bool operator==(const iterator& lhs,
                                            const iterator& rhs);
    MyDRefl_core_API friend bool operator!=(const iterator& lhs,
                                            const iterator& rhs);

    bool Valid() const noexcept { return typeiter.Valid(); }
    std::span<const Ranges::Derived> GetDeriveds() const noexcept {
      return typeiter.GetDeriveds();
    }
    ObjectView GetObjectView() const { return std::get<ObjectView>(*typeiter); }
    TypeInfo* GetTypeInfo() const { return std::get<TypeInfo*>(*typeiter); }

   private:
    void update();
    ObjectTree::iterator typeiter;
    FieldFlag flag;
    int mode;
    std::unordered_map<Name, FieldInfo>::iterator curfield;
  };

  constexpr FieldRange(ObjectView obj, FieldFlag flag) noexcept
      : objtree{ObjectTree{obj}}, flag{flag} {}
  constexpr explicit FieldRange(ObjectView obj) noexcept
      : FieldRange{obj, FieldFlag::All} {}
  constexpr explicit FieldRange(Type type) noexcept
      : FieldRange{ObjectView{type}, FieldFlag::All} {}

  iterator begin() const { return {objtree.begin(), flag}; }
  iterator end() const noexcept { return {objtree.end(), flag}; }

 private:
  ObjectTree objtree;
  FieldFlag flag;
};

template <typename T, FieldFlag flag = FieldFlag::All>
static constexpr FieldRange FieldRange_of = FieldRange{ObjectView_of<T>, flag};
}  // namespace My::MyDRefl
