#ifndef __EASYLUA_FUNCTION_H
#define __EASYLUA_FUNCTION_H

#include <tuple>

#include <lua.hpp>

#include "exception.hpp"
#include "stack.hpp"

namespace easylua
{
    /**
     * @brief Represents the result of calling a function. This class makes it possible to call a function without explicitly specifying the
     * expected return types.
     * That is, rather than writing:
     *                      int result = f<int>(x);
     * You can write:
     *                      int result = f(x);
     *
     * Instances of this class are meant to be temporary, and should not be stored. Storing an instance of this class may result in a corrupt
     * Lua stack.
     */
    class FunctionResult
    {
    public:
        /**
         * @brief Construct a new FunctionResult object
         *
         * @param state Lua state.
         * @param number_of_results The number of result that are on the stack. This will be used to pop the values from the stack when the object is destructed.
         */
        FunctionResult(lua_State *state, std::size_t number_of_results) : lua_state_(state), number_of_results_(number_of_results)
            {
                if (!lua_state_)
                    throw InvalidArgumentException("state", "cannot be null");
            }

        ~FunctionResult()
            {
                stack::Pop(lua_state_, number_of_results_);
            }

        FunctionResult(const FunctionResult &other) = delete;
        FunctionResult(FunctionResult &&other) = delete;
        FunctionResult &operator=(const FunctionResult &other) = delete;
        FunctionResult &operator=(FunctionResult &&other) = delete;

            template <typename T>
            operator T()
            {
                if (number_of_results_ == 0)
                    throw CallException("no results to convert");

                T result = stack::Get<T>(lua_state_, -1);
                return result;
            }

            template <typename... T>
            operator std::tuple<T...>()
            {
                if (number_of_results_ == 0 && sizeof...(T) > 0)
                    throw CallException("no results to convert");

                std::tuple<T...> result = stack::Get<T...>(lua_state_);
                return result;
            }

        private:
            lua_State *lua_state_;
            std::size_t number_of_results_;
        };

    namespace detail
    {
        /**
         * @brief Calls the function that is at the top of the Lua stack.
         *
         * @tparam Args The types of the arguments to pass to the function.
         * @param args The arguments to pass to the function.
         * @return FunctionResult The result of calling the function.
         * @throw TypeException If the value at the top of the stack is not a function.
         * @throw CallException If the function call fails.
         */
        template <typename... Args>
        FunctionResult CallFunction(lua_State *L, Args... args)
        {
            if (!stack::CheckType(L, -1, LUA_TFUNCTION))
                throw TypeException(-1, lua_type(L, -1), LUA_TFUNCTION);

            const int old_stack = stack::Top(L);

            // Call the function
            const int num_args = sizeof...(Args);

            // Push the arguments
            if constexpr (num_args > 0)
                stack::Push(L, args...);

            if (lua_pcall(L, num_args, LUA_MULTRET, 0) != 0)
            {
                const char *error_message = lua_tostring(L, -1);
                stack::Pop(L, 1);
                throw CallException(error_message);
            }

            const int number_of_results = 1 + stack::Top(L) - old_stack; // +1 because function was already on the stack

            // Read the results
            return FunctionResult(L, number_of_results);
        }
    };

    /** Represents a Lua function on the stack. */
    class StackFunction
    {
    public:
        StackFunction(lua_State *state, int index = -1) : lua_state_(state), index_(index)
        {
            if (!lua_state_)
                throw InvalidArgumentException("state", "cannot be null");
        }

        template <typename... Args>
        FunctionResult operator()(Args... args)
        {
            lua_pushvalue(lua_state_, index_);
            return detail::CallFunction(lua_state_, args...);
        }

    private:
        lua_State *lua_state_;
        int index_;
    };
} // namespace easylua

#endif