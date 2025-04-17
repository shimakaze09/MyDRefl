#pragma once

#if (defined(WIN32) || defined(_WIN32)) && defined(MY_MYDREFL_SHARED)
#ifdef MYCMAKE_EXPORT_MyDRefl_core
#define MyDRefl_core_API __declspec(dllexport)
#define MyDRefl_core_CLASS_API __declspec(dllexport)
#else
#define MyDRefl_core_API __declspec(dllimport)
#define MyDRefl_core_CLASS_API __declspec(dllexport)
#endif
#else
#define MyDRefl_core_API extern
#define MyDRefl_core_CLASS_API
#endif  // (defined(WIN32) || defined(_WIN32)) && !defined(MY_MYDREFL_SHARED)

#include <cstddef>

// use it in "Basic.h"
#ifndef MY_MYDREFL_INCLUDE_ALL_STD_NAME
// #define MY_MYDREFL_INCLUDE_ALL_STD_NAME
#endif  // MY_MYDREFL_INCLUDE_ALL_STD_NAME

namespace My::MyDRefl {
static constexpr std::size_t MaxArgNum = 64;
static_assert(MaxArgNum <= 256 - 2);
}  // namespace My::MyDRefl
