#ifndef __EASYLUA_STATE_VIEW_H
#define __EASYLUA_STATE_VIEW_H

#include <lua.hpp>

#include "exception.hpp"
#include "reference.hpp"
#include "stack.hpp"

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

        class get_result
        {
        public:
            get_result(lua_State* state, int index) : lua_state_(state), index_(index)
            {
            }

            get_result(const get_result& other) = delete;
            get_result(get_result&& other) = delete;
            get_result& operator=(const get_result& other) = delete;
            get_result& operator=(get_result&& other) = delete;

            template <typename T>
            operator T() const &&
            {
                return stack::get<T>(lua_state_, index_);
            }

            // TODO reference

        private:
            lua_State* lua_state_;
            int index_;
        };

        get_result operator[](const char *name) const
        {
            lua_getglobal(lua_state_, name);
            return get_result(lua_state_, -1);
        }

    protected:
        lua_State *lua_state_;
    };
} // namespace easylua

#endif