//
// Created by Admin on 11/03/2025.
//

#pragma once

#include "Attr.h"

#include <cassert>
#include <map>
#include <string>

namespace My::MyDRefl {
struct MYDREFL_DESC AttrList {
  std::map<std::string, Attr, std::less<>> data;

  bool Contains(std::string_view name) const {
    return data.find(name) != data.end();
  }

  template <typename T>
  T& Get(std::string_view name) {
    assert(Contains(name));
    return data.find(name)->second.Cast<T>();
  }
};
}  // namespace My::MyDRefl
