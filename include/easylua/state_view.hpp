#ifndef __EASYLUA_STATE_VIEW_H
#define __EASYLUA_STATE_VIEW_H

#include <lua.hpp>

#include "exception.hpp"

namespace easylua
{
    class StateView
    {
    public:
        StateView(lua_State *state) : lua_state_(state)
        {
            if (!lua_state_)
                throw InvalidArgumentException("state", "cannot be null");
        }

        lua_State *GetLuaState() const { return lua_state_; }

    protected:
        lua_State *lua_state_;
    };
} // namespace easylua

#endif