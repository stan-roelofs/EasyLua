#ifndef __EASYLUA_STACK_H
#define __EASYLUA_STACK_H

#include <lua.hpp>
#include "exception.hpp"

namespace easylua
{
    namespace stack
    {
        /**
         * @brief Get the value at the given index on the stack.
         *
         * @tparam T The type of the value to get.
         * @param L The Lua state.
         * @param index The index of the value on the stack.
         * @return T The value at the given index.
         */
        template <typename T>
        T Get(lua_State *L, int index)
        {
            if (index == 0)
                throw InvalidArgumentException("index", "cannot be 0");

            if constexpr ((std::is_integral_v<T> || std::is_same_v<T, lua_Integer>)&&!std::is_same_v<T, bool>)
            {
                if (lua_type(L, index) == LUA_TNUMBER)
                    return lua_tointeger(L, index);

                throw TypeException(index, lua_type(L, index), LUA_TNUMBER);
            }
            else if constexpr (std::is_floating_point_v<T> || std::is_same_v<T, lua_Number>)
            {
                if (lua_type(L, index) == LUA_TNUMBER)
                    return lua_tonumber(L, index);

                throw TypeException(index, lua_type(L, index), LUA_TNUMBER);
            }
            else if constexpr (std::is_same_v<T, bool>)
            {
                if (lua_type(L, index) == LUA_TBOOLEAN)
                    return lua_toboolean(L, index);

                throw TypeException(index, lua_type(L, index), LUA_TBOOLEAN);
            }
            else if constexpr (std::is_same_v<T, const char *> || std::is_same_v<T, std::string> || std::is_same_v<T, std::string_view>)
            {
                if (lua_type(L, index) == LUA_TSTRING)
                    return lua_tostring(L, index);

                throw TypeException(index, lua_type(L, index), LUA_TSTRING);
            }

            throw InvalidArgumentException("T", "is not a supported type");
        }

        /**
         * @brief Set the value at the given index on the stack.
         *
         * @tparam T The type of the value to set.
         * @param L The Lua state.
         * @param index The index of the value on the stack.
         * @param value The value to set.
         */
        template <typename T>
        void Set(lua_State *L, int index, T value)
        {
            if (index == 0)
                throw InvalidArgumentException("index", "cannot be 0");

            if constexpr (std::is_same_v<T, const char *>)
                lua_pushstring(L, value);
            else if constexpr (std::is_same_v<T, std::string>)
                lua_pushstring(L, value.c_str());
            else if constexpr (std::is_same_v<T, std::string_view>)
                lua_pushlstring(L, value.data(), value.size());
            else if constexpr (std::is_same_v<T, bool>)
                lua_pushboolean(L, value);
            else if constexpr (std::is_integral_v<T> || std::is_same_v<T, lua_Integer>)
                lua_pushinteger(L, value);
            else if constexpr (std::is_floating_point_v<T> || std::is_same_v<T, lua_Number>)
                lua_pushnumber(L, value);
            else
                throw InvalidArgumentException("T", "is not a supported type");
        }
    }

} // namespace easylua

#endif