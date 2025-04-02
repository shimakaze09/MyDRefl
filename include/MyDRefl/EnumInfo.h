//
// Created by Admin on 1/04/2025.
//

#pragma once

#include "Enumerator.h"

#include <MyTemplate/TypeID.h>

namespace My::MyDRefl {
struct EnumInfo {
  Enumerator::UnderlyingType underlyingType;
  std::unordered_map<StrID, EnumeratorInfo> enumeratorinfos;
  std::unordered_map<TypeID, SharedBlock> attrs;

  Enumerator GetEnumerator(StrID enumeratorID) const {
    return {underlyingType, enumeratorinfos.at(enumeratorID).value};
  }

  StrID GetEnumeratorStrID(Enumerator::Value value) const {
    for (const auto& [ID, info] : enumeratorinfos) {
      if (value.data_uint64 == info.value.data_uint64)
        return ID;
    }
    return {};
  }

  template <typename E>
  StrID GetEnumeratorStrID(E e) const {
    assert(underlyingType == Enumerator::UnderlyingTypeOf<E>());
    return GetEnumeratorStrID(Enumerator::ValueOf(e));
  }
};
}  // namespace My::MyDRefl
