#ifndef __LUA_WRAPPER_HPP
#define __LUA_WRAPPER_HPP

#include <lua.hpp>
#include <stdexcept>
#include <memory>
#include <iostream>
#include <string_view>
#include <tuple>

namespace lua_wrapper
{
    class OutOfMemoryException : public std::runtime_error
    {
    public:
        OutOfMemoryException(std::string_view msg) : std::runtime_error(std::string(msg)) {}
    };

    class LuaContext final
    {
    public:
        LuaContext();
        ~LuaContext();

        template <class T>
        void Write(T value)
        {
            std::cout << "Write " << value << std::endl;
        }

        void Write()
        {
        }

        template <class T, class... Args>
        void Write(T value, Args... args)
        {
            Write(value);
            Write(args...);
        }

        template <class T>
        T Read();

        template <typename... Args>
        std::tuple<Args...> ReadMultiple()
        {
            return std::tuple<Args...>(Read<Args>()...);
        }

        template <typename... ReturnTypes, typename... Parameters>
        std::tuple<ReturnTypes...> Call(const std::string_view &function_name, Parameters... parameters)
        {
            Write(parameters...);

            return ReadMultiple<ReturnTypes...>();
        }

    private:
        lua_State *lua_state_;
    };

    template <>
    inline int LuaContext::Read()
    {
        return 5;
    }

    template <>
    inline const char *LuaContext::Read()
    {
        return " abc ";
    }
}

#endif