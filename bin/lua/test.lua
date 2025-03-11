print("load test.lua")
print("load MyDRefl_lua.dll")
local MyDRefl_lua = require("MyDRefl_luad")
print(MyDRefl_lua.hello())
print("done")
print("you can use MyDRefl_lua")

print("[lua] pointer: ", MyDRefl_lua.TypeInfoMngr.GetTypeInfo(0))
local meta = getmetatable(MyDRefl_lua.TypeInfoMngr.GetTypeInfo(0))
print("TypeInfo metatable name: " .. meta.__name)

local type = MyDRefl_lua.TypeInfoMngr.GetTypeInfo(0)
local point = MyDRefl_lua.TypeInfo.New(type)
MyDRefl_lua.TypeInfo.Delete(point)

return MyDRefl_lua
