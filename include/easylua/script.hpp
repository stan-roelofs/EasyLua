#ifndef __EASYLUA_SCRIPT_H
#define __EASYLUA_SCRIPT_H

#include <lua.hpp>
#include <string>

#include "exception.hpp"

namespace easylua
{
    namespace script
    {
        enum class LoadResult
        {
            OK,
            SyntaxError,
            MemoryAllocationError,
            FileError,
            UnknownError
        };

        /**
         * @brief Load a script from a file.
         *
         * @param L The Lua state.
         * @param path The path to the script.
         * @return LoadResult The result of the load.
         */
        LoadResult LoadFile(lua_State *L, const std::string &path)
        {
            if (path.empty())
                throw InvalidArgumentException("path", "cannot be empty");

            const int result = luaL_loadfile(L, path.c_str());

            switch (result)
            {
            case 0:
                return LoadResult::OK;
            case LUA_ERRSYNTAX:
                return LoadResult::SyntaxError;
            case LUA_ERRMEM:
                return LoadResult::MemoryAllocationError;
            case LUA_ERRFILE:
                return LoadResult::FileError;
            }

            return LoadResult::UnknownError;
        }

        /**
         * @brief Load a script from a string.
         *
         * @param L The Lua state.
         * @param script The script.
         * @return LoadResult The result of the load.
         */
        LoadResult LoadString(lua_State *L, const std::string &script)
        {
            const int result = luaL_loadstring(L, script.c_str());

            switch (result)
            {
            case 0:
                return LoadResult::OK;
            case LUA_ERRSYNTAX:
                return LoadResult::SyntaxError;
            case LUA_ERRMEM:
                return LoadResult::MemoryAllocationError;
            }
        }
    }

} // namespace easylua

#endif