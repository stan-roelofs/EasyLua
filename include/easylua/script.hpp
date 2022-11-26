#ifndef __EASYLUA_SCRIPT_H
#define __EASYLUA_SCRIPT_H

#include <string>

#include <lua.hpp>

#include "exception.hpp"

namespace easylua
{
    namespace script
    {
        enum class LoadResult
        {
            kOK,
            kSyntaxError,
            kMemoryAllocationError,
            kFileError,
            kUnknownError
        };

        /**
         * @brief Load a script from a file.
         *
         * @param L The Lua state.
         * @param path The path to the script.
         * @return LoadResult The result of the load.
         */
        LoadResult FromFile(lua_State *L, const std::string &path)
        {
            if (path.empty())
                throw InvalidArgumentException("path", "cannot be empty");

            const int result = luaL_loadfile(L, path.c_str());

            switch (result)
            {
            case 0:
                return LoadResult::kOK;
            case LUA_ERRSYNTAX:
                return LoadResult::kSyntaxError;
            case LUA_ERRMEM:
                return LoadResult::kMemoryAllocationError;
            case LUA_ERRFILE:
                return LoadResult::kFileError;
            }

            return LoadResult::kUnknownError;
        }

        /**
         * @brief Load a script from a string.
         *
         * @param L The Lua state.
         * @param script The script.
         * @return LoadResult The result of the load.
         */
        LoadResult FromString(lua_State *L, const std::string &script)
        {
            const int result = luaL_loadstring(L, script.c_str());

            switch (result)
            {
            case 0:
                return LoadResult::kOK;
            case LUA_ERRSYNTAX:
                return LoadResult::kSyntaxError;
            case LUA_ERRMEM:
                return LoadResult::kMemoryAllocationError;
            }

            return LoadResult::kUnknownError;
        }

        // TODO runfile runstring
    }

} // namespace easylua

#endif