//
// Created by Admin on 11/03/2025.
//

#pragma once

#if defined(_WIN32) && defined(MY_MYDREFL_BUILD_AS_SHARED)
#if defined(MY_MYDREFL_EXPORT)
#define MYDREFL_DESC __declspec(dllexport)
#else
#define MYDREFL_DESC __declspec(dllimport)
#endif
#else
#define MYDREFL_DESC
#endif
