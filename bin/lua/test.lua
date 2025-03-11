print("load test.lua")
print("load MyDRefl_lua.dll")
local MyDRefl_lua = require("MyDRefl_lua")
print(MyDRefl_lua.hello())
print("done")
print("you can use MyDRefl_lua")
return MyDRefl_lua