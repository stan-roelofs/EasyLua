# EasyLua
This is a header-only C++ wrapper for Lua. It aims to make interacting with Lua code easier by abstracting away the Lua stack.


## Examples
```cpp
easylua::state state;
easylua::load_result = state.run("function f(a) return a, \"b\" end");
if (!load_result)
{
    std::cerr << load_result.error();
    return;
}

easylua::function f = state["f"];
std::tuple<int, std::string> result = f(123);

std::cout << std::get<0>(result) << " " << std::get<1>(result) << std::endl;
```

Using the standard Lua C API this could be implemented as follows:
```cpp
lua_State *L = luaL_newstate();
if (!luaL_dostring(L, "function f(a) return a, \"b\"; end"))
{
    std::cerr << lua_tostring(L, -1);
    return;
}

lua_getglobal(L, "f");
lua_pushnumber(L, 123);
lua_pcall(L, 1, 2, 0);

int result_int = lua_tointeger(L, -2);
const char *result_string = lua_tostring(L, -1);    

std::cout << result_int << " " << result_string << std::endl;

```