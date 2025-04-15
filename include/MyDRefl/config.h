#pragma once

#include <cstddef>

// use it in "Basic.h"
#ifndef MY_MYDREFL_INCLUDE_ALL_STD_NAME
// #define MY_MYDREFL_INCLUDE_ALL_STD_NAME
#endif  // MY_MYDREFL_INCLUDE_ALL_STD_NAME

namespace My::MyDRefl {
static constexpr std::size_t MaxArgNum = 64 - 2;
static_assert(MaxArgNum <= 256 - 2);
}  // namespace My::MyDRefl
