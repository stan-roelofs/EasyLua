#ifndef __EASYLUA_STATE_VIEW_H
#define __EASYLUA_STATE_VIEW_H

#include <lua.hpp>

#include "exception.hpp"

namespace easylua
{
    class state_view
    {
    public:
        state_view(lua_State *state) : lua_state_(state)
        {
            if (!lua_state_)
                throw invalid_argument("state", "cannot be null");
        }

        lua_State *get_state() const { return lua_state_; }

    protected:
        lua_State *lua_state_;
    };
} // namespace easylua

#endif