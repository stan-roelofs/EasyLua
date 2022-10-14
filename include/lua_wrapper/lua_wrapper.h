#ifndef __LUA_WRAPPER_HPP
#define __LUA_WRAPPER_HPP

#include <lua.hpp>
#include <stdexcept>
#include <memory>
#include <string>

namespace lua_wrapper
{
    class OutOfMemoryException : public std::runtime_error
    {
    public:
        OutOfMemoryException(std::string msg) : std::runtime_error(msg) {}
    };

    class LuaContext final
    {
    public:
        LuaContext();
        ~LuaContext();

    private:
        lua_State *lua_state_;
    };
}

#endif