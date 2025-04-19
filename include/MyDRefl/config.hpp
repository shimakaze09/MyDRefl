#pragma once

#if (defined(WIN32) || defined(_WIN32)) && defined(MY_MYDREFL_SHARED)
#ifdef MYCMAKE_EXPORT_MyDRefl_core
#define MyDRefl_core_API __declspec(dllexport)
#else
#define MyDRefl_core_API __declspec(dllimport)
#endif
#else
#define MyDRefl_core_API
#endif  // (defined(WIN32) || defined(_WIN32)) && !defined(MY_MYDREFL_SHARED)

#include <cstddef>

// use it in "Basic.h"
#ifndef MY_MYDREFL_INCLUDE_ALL_STD_NAME
// #define MY_MYDREFL_INCLUDE_ALL_STD_NAME
#endif  // MY_MYDREFL_INCLUDE_ALL_STD_NAME

namespace Smkz::MyDRefl {
static constexpr std::size_t MaxArgNum = 64;
static_assert(MaxArgNum <= 256 - 2);
}  // namespace Smkz::MyDRefl
