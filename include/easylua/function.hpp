#ifndef __EASYLUA_FUNCTION_H
#define __EASYLUA_FUNCTION_H

#include <tuple>

#include <lua.hpp>

#include "exception.hpp"
#include "stack.hpp"

namespace easylua
{
    /** Represents a Lua function. */
    class Function
    {
    public:
        Function(lua_State *state) : lua_state_(state)
        {
            if (!lua_state_)
                throw InvalidArgumentException("state", "cannot be null");
        }

        /**
         *
         */
        class Result
        {
        public:
            Result(lua_State *state) : lua_state_(state)
            {
                if (!lua_state_)
                    throw InvalidArgumentException("state", "cannot be null");
            }
            Result(const Result &other) = delete;
            Result(Result &&other) = delete;
            Result &operator=(const Result &other) = delete;
            Result &operator=(Result &&other) = delete;

            template <typename T>
            operator T()
            {
                return stack::Pop<T>(lua_state_);
            }

        private:
            lua_State *lua_state_;
        };

        template <typename... Args>
        Result Call(Args... args)
        {
            // Call the function
            const int num_args = sizeof...(Args);

            // Push the arguments
            if constexpr (num_args > 0)
                stack::Push(lua_state_, args...);

            const int error = lua_pcall(lua_state_, num_args, LUA_MULTRET, 0);
            if (error)
            {
                const char *error_message = lua_tostring(lua_state_, -1);
                lua_pop(lua_state_, 1);
                throw CallException(error_message);
            }

            // Read the results
            return Result(lua_state_);
        }

        template <typename... Args>
        Result operator()(Args... args)
        {
            return Call(args...);
        }

    private:
        lua_State *lua_state_;
    };
} // namespace easylua

#endif