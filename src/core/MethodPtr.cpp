//
// Created by Admin on 1/04/2025.
//

#include <MyDRefl/MethodPtr.h>

using namespace My::MyDRefl;

ParamList::ParamList(std::vector<Parameter> params) : params{params} {
  offsets.reserve(params.size());

  size_t curOffset = 0;
  for (const auto& param : params) {
    if (param.alignment > alignment)
      alignment = param.alignment;

    curOffset = (curOffset + param.alignment - 1) / param.alignment;
    offsets.push_back(curOffset);
    curOffset += param.size;
  }

  size = curOffset;
}

bool ParamList::IsConpatibleWith(Span<TypeID> typeIDs) const noexcept {
  if (params.size() != typeIDs.size())
    return false;

  for (size_t i = 0; i < params.size(); i++) {
    if (params[i].typeID != typeIDs[i])
      return false;
  }

  return true;
}
