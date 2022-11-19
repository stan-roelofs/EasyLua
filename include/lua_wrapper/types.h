#ifndef __LUA_WRAPPER_TYPES_H
#define __LUA_WRAPPER_TYPES_H

#include <lua.hpp>

namespace lua_wrapper
{
    using string = const char *;
    using integer = int;
    using number = lua_Number;
    using boolean = bool;
} // namespace lua_wrapper

#endif