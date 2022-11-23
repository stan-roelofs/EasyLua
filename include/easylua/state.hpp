#ifndef __EASYLUA_STATE_H
#define __EASYLUA_STATE_H

#include <lua.hpp>

#include "state_view.hpp"

namespace easylua
{
    class State final : public StateView
    {
    public:
        State() : StateView(luaL_newstate())
        {
        }

        ~State()
        {
            if (lua_state_)
                lua_close(lua_state_);

            lua_state_ = nullptr;
        }

        State(const State &other) = delete;
        State &operator=(const State &other) = delete;

        State(State &&other) : StateView(other.lua_state_)
        {
            other.lua_state_ = nullptr;
        }

        State &operator=(State &&other)
        {
            std::swap(lua_state_, other.lua_state_);
            return *this;
        }
    };
} // namespace easylua

#endif