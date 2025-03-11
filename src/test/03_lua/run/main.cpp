//
// Created by Admin on 11/03/2025.
//

#include <lua.hpp>

#include "point.h"

int main() {
  My::Point_init();
  My::Point_test();

  char buff[256];
  int error;
  lua_State* L = luaL_newstate(); /* opens Lua */
  luaL_openlibs(L);               /* opens the standard libraries */

  // 1. load MyDRefl_lua
  error = luaL_loadstring(L, "MyDRefl_lua = require(\"lua/test\")") ||
          lua_pcall(L, 0, 0, 0);
  if (error) {
    fprintf(stderr, "%s\n", lua_tostring(L, -1));
    lua_pop(L, 1); /* pop error message from the stack */
  }

  // 2. run CLI
  while (fgets(buff, sizeof(buff), stdin) != NULL) {
    error = luaL_loadstring(L, buff) || lua_pcall(L, 0, 0, 0);
    if (error) {
      fprintf(stderr, "%s\n", lua_tostring(L, -1));
      lua_pop(L, 1); /* pop error message from the stack */
    }
  }
  lua_close(L);
  return 0;
}
