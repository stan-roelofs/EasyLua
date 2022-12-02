#ifndef __EASYLUA_REFERENCE_H
#define __EASYLUA_REFERENCE_H

#include <lua.hpp>

namespace easylua
{
    class reference
    {
    public:
        reference(lua_State *state, int index) : lua_state_(state), reference_(LUA_NOREF)
        {
            if (!lua_state_)
                throw invalid_argument("state", "cannot be null");

            reference_ = luaL_ref(lua_state_, LUA_REGISTRYINDEX);
        }

        reference(const reference &other) : lua_state_(other.lua_state_), reference_(LUA_NOREF)
        {
            if (other.reference_ != LUA_NOREF)
            {
                lua_rawgeti(lua_state_, LUA_REGISTRYINDEX, other.reference_);
                reference_ = luaL_ref(lua_state_, LUA_REGISTRYINDEX);
            }
        }

        reference(reference &&other) : lua_state_(other.lua_state_), reference_(other.reference_)
        {
            other.reference_ = LUA_NOREF;
        }

        reference &operator=(const reference &other)
        {
            if (this != &other)
            {
                if (reference_ != LUA_NOREF)
                    luaL_unref(lua_state_, LUA_REGISTRYINDEX, reference_);

                lua_state_ = other.lua_state_;
                reference_ = LUA_NOREF;

                if (other.reference_ != LUA_NOREF)
                {
                    lua_rawgeti(lua_state_, LUA_REGISTRYINDEX, other.reference_);
                    reference_ = luaL_ref(lua_state_, LUA_REGISTRYINDEX);
                }
            }

            return *this;
        }

        reference &operator=(reference &&other)
        {
            if (this != &other)
            {
                if (reference_ != LUA_NOREF)
                    luaL_unref(lua_state_, LUA_REGISTRYINDEX, reference_);

                lua_state_ = other.lua_state_;
                reference_ = other.reference_;

                other.reference_ = LUA_NOREF;
            }

            return *this;
        }

        ~reference()
        {
            if (reference_ != LUA_NOREF)
                luaL_unref(lua_state_, LUA_REGISTRYINDEX, reference_);
        }

        void push() const
        {
            if (reference_ == LUA_NOREF)
                throw runtime_error("cannot push a reference that is not valid");

            lua_rawgeti(lua_state_, LUA_REGISTRYINDEX, reference_);
        }

    private:
        lua_State *lua_state_;
        int reference_;
    };
} // namespace easylua

#endif