#ifndef __EASYLUA_STATE_H
#define __EASYLUA_STATE_H

#include <lua.hpp>

#include "state_view.hpp"

namespace easylua
{
    class state final : public state_view
    {
    public:
        state() : state_view(luaL_newstate())
        {
            // TODO set panic?
        }

        ~state()
        {
            if (lua_state_)
                lua_close(lua_state_);

            lua_state_ = nullptr;
        }

        state(const state &other) = delete;
        state &operator=(const state &other) = delete;

        state(state &&other) : state_view(other.lua_state_)
        {
            other.lua_state_ = nullptr;
        }

        state &operator=(state &&other)
        {
            std::swap(lua_state_, other.lua_state_);
            return *this;
        }
    };
} // namespace easylua

#endif