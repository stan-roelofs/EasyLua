#ifndef __LUA_WRAPPER_H
#define __LUA_WRAPPER_H

#include <lua.hpp>
#include <stdexcept>
#include <memory>
#include <iostream>
#include <string_view>
#include <tuple>

#include "types.h"

#define Log(...) (printf(__VA_ARGS__));

namespace lua_wrapper
{
    class Exception : public std::runtime_error
    {
    protected:
        Exception(std::string msg) : std::runtime_error(msg) {}
    };

    class OutOfMemoryException : public Exception
    {
    public:
        OutOfMemoryException(std::string msg) : Exception(msg) {}
    };

    class TypeException : public Exception
    {
    public:
        TypeException(std::string msg) : Exception(msg) {}
    };

    class CallException : public Exception
    {
    public:
        CallException(std::string msg) : Exception(msg) {}
    };

    class LuaContext final
    {
    public:
        LuaContext();
        ~LuaContext();

        lua_State *GetRawState() { return lua_state_; }

        void GetGlobal(string name)
        {
            lua_getglobal(lua_state_, name);
        }

        void Write() {}

        template <class T>
        void Write(T value);

        template <class T, class... Args>
        void Write(T value, Args... args)
        {
            Write(value);
            Write(args...);
        }

        template <class T>
        T Read(int index = -1);

        template <class T>
        T Pop()
        {
            const T result = Read<T>(-1);
            lua_pop(lua_state_, 1);
            return result;
        }

        template <typename... Args>
        std::tuple<Args...> ReadMultiple()
        {
            int index = -1;
            return std::tuple<Args...>(Read<Args>(index--)...);
        }

        template <typename... ReturnTypes, typename... Parameters>
        std::tuple<ReturnTypes...> Call(string function_name, Parameters... parameters)
        {
            const std::size_t number_of_parameters = sizeof...(parameters);
            const std::size_t number_of_returns = sizeof...(ReturnTypes);

            Log("Calling function %s with %lu parameters and %lu return values\n", function_name, number_of_parameters, number_of_returns);

            if (lua_getglobal(lua_state_, function_name) != LUA_TFUNCTION)
                throw TypeException(std::string(function_name) + " is not a Lua function!");

            Write(parameters...);

            if (lua_pcall(lua_state_, number_of_parameters, number_of_returns, 0) != 0)
                throw CallException(std::string("Call failed : ") + std::string(Read<string>()));

            const std::tuple<ReturnTypes...> result = ReadMultiple<ReturnTypes...>();
            lua_pop(lua_state_, static_cast<int>(number_of_returns));
            return result;
        }

    private:
        lua_State *lua_state_;
    };

    template <>
    inline integer LuaContext::Read(int index)
    {
        if (lua_type(lua_state_, index) != LUA_TNUMBER)
            throw TypeException("Expected integer, got: " + std::string(lua_typename(lua_state_, lua_type(lua_state_, index))));
        return lua_tointeger(lua_state_, index);
    }

    template <>
    inline string LuaContext::Read(int index)
    {
        if (lua_type(lua_state_, index) != LUA_TSTRING)
            throw TypeException("Expected string, got: " + std::string(lua_typename(lua_state_, lua_type(lua_state_, index))));
        return lua_tostring(lua_state_, index);
    }

    template <>
    inline number LuaContext::Read(int index)
    {
        if (lua_type(lua_state_, index) != LUA_TNUMBER)
            throw TypeException("Expected number, got: " + std::string(lua_typename(lua_state_, lua_type(lua_state_, index))));
        return lua_tonumber(lua_state_, index);
    }

    template <>
    inline boolean LuaContext::Read(int index)
    {
        if (lua_type(lua_state_, index) != LUA_TBOOLEAN)
            throw TypeException("Expected boolean, got: " + std::string(lua_typename(lua_state_, lua_type(lua_state_, index))));
        return lua_toboolean(lua_state_, index);
    }

    template <>
    inline void LuaContext::Write(string value)
    {
        lua_pushstring(lua_state_, value);
    }

    template <>
    inline void LuaContext::Write(number value)
    {
        lua_pushnumber(lua_state_, value);
    }

    template <>
    inline void LuaContext::Write(integer value)
    {
        lua_pushinteger(lua_state_, value);
    }

    template <>
    inline void LuaContext::Write(boolean value)
    {
        lua_pushboolean(lua_state_, value);
    }
}

#endif