#pragma once

#if (defined(WIN32) || defined(_WIN32)) && defined(MY_MYDREFL_SHARED)
#ifdef MYCMAKE_EXPORT_MyDRefl_ext_Bootstrap
#define MyDRefl_ext_Bootstrap_API __declspec(dllexport)
#else
#define MyDRefl_ext_Bootstrap_API __declspec(dllimport)
#endif
#else
#define MyDRefl_ext_Bootstrap_API extern
#endif  // (defined(WIN32) || defined(_WIN32)) && defined(MY_MYDREFL_SHARED)

#ifdef __cplusplus
extern "C" {
#endif

MyDRefl_ext_Bootstrap_API void My_MyDRefl_ext_Bootstrap();

#ifdef __cplusplus
}
#endif
