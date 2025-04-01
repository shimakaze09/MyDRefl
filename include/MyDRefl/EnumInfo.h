//
// Created by Admin on 1/04/2025.
//

#pragma once

#include "Enumerator.h"

namespace My::MyDRefl {
struct EnumInfo {
  std::unordered_map<size_t, std::any> attrs;
  Enumerator::UnderlyingType underlyingType;
  std::unordered_map<size_t, EnumeratorInfo> enumeratorinfos;

  Enumerator GetEnumerator(size_t ID) const {
    return {underlyingType, enumeratorinfos.at(ID).value};
  }

  size_t GetID(Enumerator::Value value) const {
    for (const auto& [ID, info] : enumeratorinfos) {
      if (value.data_uint64 == info.value.data_uint64)
        return ID;
    }
    return static_cast<size_t>(-1);
  }

  template <typename E>
  size_t GetID(E e) const {
    assert(underlyingType == Enumerator::UnderlyingTypeOf<E>());
    return GetID(Enumerator::ValueOf(e));
  }
};
}  // namespace My::MyDRefl
