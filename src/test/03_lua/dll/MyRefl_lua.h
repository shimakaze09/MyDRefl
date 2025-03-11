//
// Created by Admin on 11/03/2025.
//

#pragma once

#include <lua.hpp>

#ifdef MY_EXPORT_MYDREFL_LUA
#define MYDREFL_LUA_DESC __declspec(dllexport)
#else
#define MYDREFL_LUA_DESC __declspec(dllimport)
#endif

extern "C" MYDREFL_LUA_DESC int luaopen_MyDRefl_lua(lua_State* L);
