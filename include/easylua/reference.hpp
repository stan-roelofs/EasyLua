#ifndef __EASYLUA_REFERENCE_H
#define __EASYLUA_REFERENCE_H

#include <lua.hpp>

#include "exception.hpp"

namespace easylua
{
    class reference
    {
    public:
        virtual ~reference() = default;

        /// @brief Pushes the referenced value onto the stack.
        virtual void push() const = 0;

    protected:
        reference(lua_State *state, int index, int expected_type) : lua_state_(state)
        {
            if (!state)
                throw invalid_argument("state", "cannot be null");

            if (index == 0)
                throw invalid_argument("index", "cannot be 0");

            if (lua_type(state, index) != expected_type)
                throw type_error(index, lua_type(state, index), expected_type);
        }

        lua_State *lua_state_;
    };

    class unsafe_reference : public reference
    {
    public:
        void push() const override
        {
            lua_pushvalue(lua_state_, index_);
        }

    protected:
        unsafe_reference(lua_State *state, int index, int expected_type) : reference(state, index, expected_type), index_(index)
        {
        }

        int index_;
    };

    class safe_reference : public reference
    {
    public:
        void push() const override
        {
            if (reference_ == LUA_NOREF)
                throw runtime_error("cannot push a safe_reference that is not valid");

            lua_rawgeti(lua_state_, LUA_REGISTRYINDEX, reference_);
        }

    protected:
        safe_reference(lua_State *state, int index, int expected_type) : reference(state, index, expected_type), reference_(LUA_NOREF)
        {
            reference_ = luaL_ref(lua_state_, LUA_REGISTRYINDEX);
        }

        // safe_reference(const safe_reference &other) : reference(other), reference_(LUA_NOREF)
        // {
        //     if (other.reference_ != LUA_NOREF)
        //     {
        //         lua_rawgeti(lua_state_, LUA_REGISTRYINDEX, other.reference_);
        //         reference_ = luaL_ref(lua_state_, LUA_REGISTRYINDEX);
        //     }
        // }

        // safe_reference(safe_reference &&other) : reference(other), reference_(other.reference_)
        // {
        //     other.reference_ = LUA_NOREF;
        // }

        // safe_reference &operator=(const safe_reference &other)
        // {
        //     if (this != &other)
        //     {
        //         if (reference_ != LUA_NOREF)
        //             luaL_unref(lua_state_, LUA_REGISTRYINDEX, reference_);

        //         lua_state_ = other.lua_state_;
        //         reference_ = LUA_NOREF;

        //         if (other.reference_ != LUA_NOREF)
        //         {
        //             lua_rawgeti(lua_state_, LUA_REGISTRYINDEX, other.reference_);
        //             reference_ = luaL_ref(lua_state_, LUA_REGISTRYINDEX);
        //         }
        //     }

        //     return *this;
        // }

        // safe_reference &operator=(safe_reference &&other)
        // {
        //     if (this != &other)
        //     {
        //         if (reference_ != LUA_NOREF)
        //             luaL_unref(lua_state_, LUA_REGISTRYINDEX, reference_);

        //         lua_state_ = other.lua_state_;
        //         reference_ = other.reference_;

        //         other.reference_ = LUA_NOREF;
        //     }

        //     return *this;
        // }

        ~safe_reference()
        {
            if (reference_ != LUA_NOREF)
                luaL_unref(lua_state_, LUA_REGISTRYINDEX, reference_);
        }

        int reference_;
    };
} // namespace easylua

#endif