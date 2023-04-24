#ifndef __EASYLUA_STATE_VIEW_H
#define __EASYLUA_STATE_VIEW_H

#include <lua.hpp>

#include "exception.hpp"
#include "reference.hpp"
#include "script.hpp"
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

        state_view(const state_view &other) = delete;
        state_view(state_view &&other)
        {
            std::swap(lua_state_, other.lua_state_);
        }
        state_view &operator=(const state_view &other) = delete;
        state_view &operator=(state_view &&other)
        {
            std::swap(lua_state_, other.lua_state_);
            return *this;
        }

        lua_State *get_state() const { return lua_state_; }

        operator lua_State *() const { return lua_state_; }

        script::load_result load(const std::string &code)
        {
            return script::load_string(lua_state_, code);
        }

        script::load_result load_file(const std::string &filename)
        {
            return script::load_file(lua_state_, filename);
        }

        script::load_result run(const std::string &code)
        {
            return script::run_string(lua_state_, code);
        }

        script::load_result run_file(const std::string &filename)
        {
            return script::run_file(lua_state_, filename);
        }

        class get_result
        {
        public:
            get_result(lua_State *state, const char *name) : lua_state_(state), name_(name)
            {
            }

            get_result(const get_result &other) = delete;
            get_result(get_result &&other) = delete;
            get_result &operator=(const get_result &other) = delete;
            get_result &operator=(get_result &&other) = delete;

            template <typename T>
            operator T() const
            {
                lua_getglobal(lua_state_, name_.c_str());
                return stack::get<T>(lua_state_, -1);
            }

            template <typename T>
            void operator=(T value) const
            {
                stack::push<T>(lua_state_, std::forward<T>(value));
                lua_setglobal(lua_state_, name_.c_str());
            }

            // template <typename T>
            // bool operator==(const T &value) const
            // {
            //     lua_getglobal(lua_state_, name_.c_str());
            //     return stack::get<T>(lua_state_, -1) == value;
            // }

        private:
            lua_State *lua_state_;
            std::string name_;
        };

        get_result operator[](const char *name) const
        {
            return get_result(lua_state_, name);
        }

    protected:
        lua_State *lua_state_;
    };
} // namespace easylua

#endif