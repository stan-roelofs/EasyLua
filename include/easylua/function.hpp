#ifndef __EASYLUA_FUNCTION_H
#define __EASYLUA_FUNCTION_H

#include <tuple>

#include <lua.hpp>

#include "exception.hpp"
#include "reference.hpp"
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
    class function_result
    {
    public:
        /**
         * @brief Construct a new function_result object
         *
         * @param state Lua state.
         * @param number_of_results The number of result that are on the stack. This will be used to pop the values from the stack when the object is destructed.
         */
        function_result(lua_State *state, std::size_t number_of_results) : lua_state_(state), number_of_results_(number_of_results)
        {
            if (!lua_state_)
                throw invalid_argument("state", "cannot be null");
        }

        ~function_result()
        {
            stack::pop(lua_state_, number_of_results_);
        }

        function_result(const function_result &other) = delete;
        function_result(function_result &&other) = delete;
        function_result &operator=(const function_result &other) = delete;
        function_result &operator=(function_result &&other) = delete;

        template <typename T>
        operator T()
        {
            if (number_of_results_ == 0)
                throw runtime_error("no results to convert");

            T result = stack::get<T>(lua_state_, -1);
            return result;
        }

        template <typename... T>
        operator std::tuple<T...>()
        {
            if (number_of_results_ == 0 && sizeof...(T) > 0)
                throw runtime_error("no results to convert");

            std::tuple<T...> result = stack::get<T...>(lua_state_);
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
         * @return function_result The result of calling the function.
         * @throw type_error If the value at the top of the stack is not a function.
         * @throw runtime_error If the function call fails.
         */
        template <typename... Args>
        function_result call_function(lua_State *L, Args... args)
        {
            if (!stack::check_type(L, -1, LUA_TFUNCTION))
                throw type_error(-1, lua_type(L, -1), LUA_TFUNCTION);

            const int old_stack = stack::get_top(L);

            // Call the function
            const int num_args = sizeof...(Args);

            // push the arguments
            if constexpr (num_args > 0)
                stack::push(L, args...);

            if (lua_pcall(L, num_args, LUA_MULTRET, 0) != 0)
            {
                const char *error_message = lua_tostring(L, -1);
                stack::pop(L, 1);
                throw runtime_error(error_message);
            }

            const int number_of_results = 1 + stack::get_top(L) - old_stack; // +1 because function was already on the stack

            // Read the results
            return function_result(L, number_of_results);
        }
    };

    /** Represents a Lua function on the stack. */
    class unsafe_function_reference : public unsafe_reference
    {
    public:
        unsafe_function_reference(lua_State *state, int index = -1) : unsafe_reference(state, index, LUA_TFUNCTION)
        {
        }

        template <typename... Args>
        function_result operator()(Args... args)
        {
            push();
            return detail::call_function(lua_state_, args...);
        }
    };

    class safe_function_reference : public safe_reference
    {
    public:
        safe_function_reference(lua_State *state, int index = -1) : safe_reference(state, index, LUA_TFUNCTION)
        {
        }

        template <typename... Args>
        function_result operator()(Args... args)
        {
            push();
            return detail::call_function(lua_state_, args...);
        }
    };
} // namespace easylua

#endif