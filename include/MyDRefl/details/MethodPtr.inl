//
// Created by Admin on 2/04/2025.
//

#pragma once

namespace My::MyDRefl {
inline ObjectPtr ArgsView::At(size_t idx) const noexcept {
  assert(idx < paramList.GetParameters().size());
  return {paramList.GetParameters()[idx].typeID,
          forward_offset(buffer, paramList.GetOffsets()[idx])};
}
}  // namespace My::MyDRefl
