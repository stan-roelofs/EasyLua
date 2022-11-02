#include <lua_wrapper/lua_wrapper.h>

#include <lauxlib.h>
#include <lualib.h>

#include <type_traits>

namespace lua_wrapper
{
    LuaContext::LuaContext()
    {
        lua_state_ = luaL_newstate();
        if (!lua_state_)
            throw OutOfMemoryException("Could not create Lua state");
    }

    LuaContext::~LuaContext()
    {
        lua_close(lua_state_);
    }
}