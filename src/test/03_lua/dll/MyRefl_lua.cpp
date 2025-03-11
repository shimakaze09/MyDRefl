//
// Created by Admin on 11/03/2025.
//

#include <MyDRefl/MyDRefl.h>

#include <iostream>

#include "MyDRefl_lua.h"

using namespace My;
using namespace std;

int hello(lua_State* L) {
  cout << "[cpp] : hello world!" << endl;
  lua_pushstring(L, "[lua] : hello world!");
  return 1;
}

namespace MyDRefl_lua {
struct MetaName {
  static constexpr const char TypeInfo[] = "MyDRefl_lua.TypeInfo";
  static constexpr const char Object[] = "MyDRefl_lua.Object";
};
namespace TypeInfoMngr {
int GetTypeInfo(lua_State* L) {
  auto id = static_cast<size_t>(lua_tointeger(L, 1));
  auto typeinfo = MyDRefl::TypeInfoMngr::Instance().GetTypeInfo(id);
  cout << typeinfo << endl;
  auto typeinfo_lua = lua_newuserdata(L, sizeof(MyDRefl::TypeInfo*));
  memcpy(typeinfo_lua, &typeinfo, sizeof(MyDRefl::TypeInfo*));
  luaL_getmetatable(L, MetaName::TypeInfo);
  lua_setmetatable(L, -2);
  return 1;
}
}  // namespace TypeInfoMngr
namespace TypeInfo {
int New(lua_State* L) {
  auto pptypeinfo =
      (MyDRefl::TypeInfo**)luaL_checkudata(L, 1, MetaName::TypeInfo);
  auto obj = (*pptypeinfo)->New();
  auto obj_lua = lua_newuserdata(L, sizeof(MyDRefl::Object));
  memcpy(obj_lua, &obj, sizeof(MyDRefl::Object));
  luaL_getmetatable(L, MetaName::Object);
  lua_setmetatable(L, -2);
  return 1;
}

int Delete(lua_State* L) {
  auto obj = (MyDRefl::Object*)luaL_checkudata(L, 1, MetaName::Object);
  MyDRefl::TypeInfo::Delete(*obj);
  return 0;
}
}  // namespace TypeInfo
}  // namespace MyDRefl_lua

void Register_TypeInfoMngr(lua_State* L) {
  lua_newtable(L);  // MyDRefl_lua.TypeInfoMngr
  const struct luaL_Reg reg_TypeInfoMngr[] = {
      {"GetTypeInfo", MyDRefl_lua::TypeInfoMngr::GetTypeInfo}, {NULL, NULL}};
  luaL_setfuncs(L, reg_TypeInfoMngr, 0);
  lua_setfield(L, -2, "TypeInfoMngr");
}

void Register_TypeInfo(lua_State* L) {
  lua_newtable(L);  // MyDRefl_lua.TypeInfo
  const struct luaL_Reg reg_TypeInfo[] = {
      {"New", MyDRefl_lua::TypeInfo::New},
      {"Delete", MyDRefl_lua::TypeInfo::Delete},
      {NULL, NULL}};
  luaL_setfuncs(L, reg_TypeInfo, 0);
  lua_setfield(L, -2, "TypeInfo");
}

int luaopen_MyDRefl_luad(lua_State* L) {
  luaL_newmetatable(L, MyDRefl_lua::MetaName::TypeInfo);
  luaL_newmetatable(L, MyDRefl_lua::MetaName::Object);
  lua_pop(L, 2);

  const struct luaL_Reg reg_MyDRefl_lua[] = {{"hello", hello}, {NULL, NULL}};
  luaL_newlib(L, reg_MyDRefl_lua);
  Register_TypeInfoMngr(L);
  Register_TypeInfo(L);
  return 1;
}
