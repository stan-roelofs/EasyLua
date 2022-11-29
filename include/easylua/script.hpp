#ifndef __EASYLUA_SCRIPT_H
#define __EASYLUA_SCRIPT_H

#include <string>

#include <lua.hpp>

#include "exception.hpp"

namespace easylua
{
    namespace script
    {
        class load_result
        {
        public:
            enum class result
            {
                ok,
                syntax_error,
                memory_error,
                file_error,
                error
            };

            load_result(result result, std::string error_message = "") : result_(result), error_message_(error_message)
            {
            }

            result get_result() { return result_; }
            const std::string &get_error_message() { return error_message_; }

            bool operator()() { return result_ == result::ok; }

        private:
            result result_;
            std::string error_message_;
        };

        /**
         * @brief Load a script from a file.
         *
         * @param L The Lua state.
         * @param path The path to the script.
         * @return load_result The result of the load.
         */
        load_result from_file(lua_State *L, const std::string &path)
        {
            if (path.empty())
                throw invalid_argument("path", "cannot be empty");

            const int result = luaL_loadfile(L, path.c_str());
            if (result == 0)
                return load_result(load_result::result::ok, "");

            const std::string error_message = lua_tostring(L, -1);

            switch (result)
            {
            case LUA_ERRSYNTAX:
                return load_result(load_result::result::syntax_error, error_message);
            case LUA_ERRMEM:
                return load_result(load_result::result::memory_error, error_message);
            case LUA_ERRFILE:
                return load_result(load_result::result::file_error, error_message);
            }

            return load_result(load_result::result::error, error_message);
        }

        /**
         * @brief Load a script from a string.
         *
         * @param L The Lua state.
         * @param script The script.
         * @return load_result The result of the load.
         */
        load_result from_string(lua_State *L, const std::string &script)
        {
            const int result = luaL_loadstring(L, script.c_str());

            if (result == 0)
                return load_result(load_result::result::ok, "");

            const std::string error_message = lua_tostring(L, -1);

            switch (result)
            {
            case LUA_ERRSYNTAX:
                return load_result(load_result::result::syntax_error, error_message);
            case LUA_ERRMEM:
                return load_result(load_result::result::memory_error, error_message);
            case LUA_ERRFILE:
                return load_result(load_result::result::file_error, error_message);
            }

            return load_result(load_result::result::error, error_message);
        }

        // TODO runfile runstring
    }

} // namespace easylua

#endif