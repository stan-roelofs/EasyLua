#ifndef __EASYLUA_STACK_H
#define __EASYLUA_STACK_H

#include <lua.hpp>

#include "exception.hpp"
#include "reference.hpp"
#include "types.hpp"

namespace easylua
{
    // TODO should this throw an exception or return an optional<T>?
    namespace stack
    {
        /**
         * @brief get the value at the given index on the stack.
         *
         * @tparam T The type of the value to get.
         * @param L The Lua state.
         * @param index The index of the value on the stack.
         * @return T The value at the given index.
         */
        template <typename T>
        T get(lua_State *L, int index = -1)
        {
            if (index == 0)
                throw invalid_argument("index", "cannot be 0");

            if constexpr ((std::is_integral_v<T> || std::is_same_v<T, lua_Integer>)&&!std::is_same_v<T, bool>)
            {
                if (lua_type(L, index) == LUA_TNUMBER)
                    return lua_tointeger(L, index);

                throw type_error(index, lua_type(L, index), LUA_TNUMBER);
            }
            else if constexpr (std::is_floating_point_v<T> || std::is_same_v<T, lua_Number>)
            {
                if (lua_type(L, index) == LUA_TNUMBER)
                    return lua_tonumber(L, index);

                throw type_error(index, lua_type(L, index), LUA_TNUMBER);
            }
            else if constexpr (std::is_same_v<T, bool>)
            {
                if (lua_type(L, index) == LUA_TBOOLEAN)
                    return lua_toboolean(L, index);

                throw type_error(index, lua_type(L, index), LUA_TBOOLEAN);
            }
            else if constexpr (std::is_same_v<T, const char *> || std::is_same_v<T, std::string> || std::is_same_v<T, std::string_view>)
            {
                if (lua_type(L, index) == LUA_TSTRING)
                    return lua_tostring(L, index);

                throw type_error(index, lua_type(L, index), LUA_TSTRING);
            }
            else if constexpr (std::is_same_v<T, nil_t>)
            {
                if (lua_type(L, index) == LUA_TNIL)
                    return nil;

                throw type_error(index, lua_type(L, index), LUA_TNIL);
            }
            else if constexpr (std::is_base_of_v<unsafe_reference, T> || std::is_base_of_v<safe_reference, T>)
            {
                return T(L, index);
            }
            else if constexpr (std::is_same_v<T, lua_CFunction>)
            {
                if (lua_type(L, index) == LUA_TFUNCTION)
                    return lua_tocfunction(L, index);

                throw type_error(index, lua_type(L, index), LUA_TFUNCTION);
            }
            else
                static_assert(!sizeof(T *), "Unsupported type");
        }

        template <typename... T, typename std::enable_if_t<(sizeof...(T) >= 2), bool> = true>
        std::tuple<T...> get(lua_State *L)
        {
            int index = -1;
            return std::tuple<T...>(get<T>(L, index--)...);
        }

        /**
         * @brief push the given value onto the stack.
         *
         * @tparam T The type of the value to set.
         * @param L The Lua state.
         * @param value The value to set.
         */
        template <typename T>
        void push(lua_State *L, T value)
        {
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
            else if constexpr (std::is_same_v<T, nil_t>)
                lua_pushnil(L);
            else if constexpr (std::is_base_of_v<unsafe_reference, T> || std::is_base_of_v<safe_reference, T>)
                value.push();
            else if constexpr (std::is_same_v<T, lua_CFunction>)
                lua_pushcfunction(L, value);
            else
                static_assert(!sizeof(T *), "Unsupported type");
        }

        template <typename Arg, typename... Args>
        void push(lua_State *L, Arg arg, Args... args)
        {
            push(L, arg);
            push(L, args...);
        }

        /**
         * @brief Pops values from the Lua stack.
         * @param L The Lua state.
         * @param count The number of values to pop.
         */
        inline void pop(lua_State *L, int count = 1)
        {
            lua_pop(L, count);
        }

        /**
         * @brief Check if the value at the given index on the stack is of the given type.
         *
         * @param L The Lua state.
         * @param index The index of the value on the stack.
         * @param type The expected Lua type.
         * @return true If the value at the given index on the stack is of the given type.
         *         false If the value at the given index on the stack is not of the given type.
         */
        inline bool check_type(lua_State *L, int index, int type)
        {
            return lua_type(L, index) == type;
        }

        inline int get_top(lua_State *L)
        {
            return lua_gettop(L);
        }
    } // namespace stack
} // namespace easylua

#endif