#include <easylua/state.hpp>

#include <gtest/gtest.h>

TEST(state_view, constructor_throws_on_null_state)
{
    EXPECT_THROW(easylua::state_view(nullptr), easylua::invalid_argument);
}

TEST(state_view, constructor)
{
    lua_State *L = luaL_newstate();
    easylua::state_view state(L);
    EXPECT_EQ(L, state.get_state());
    lua_close(L);
}

TEST(state_view, copy_constructor)
{
    lua_State *L = luaL_newstate();
    easylua::state_view state(L);
    easylua::state_view state2(state);
    EXPECT_EQ(L, state2.get_state());
    lua_close(L);
}

TEST(state_view, copy_assignment)
{
    lua_State *L = luaL_newstate();
    easylua::state_view state(L);
    easylua::state_view state2 = state;
    EXPECT_EQ(L, state2.get_state());
    lua_close(L);
}

TEST(state_view, move_constructor)
{
    lua_State *L = luaL_newstate();
    easylua::state_view state(L);
    easylua::state_view state2(std::move(state));
    EXPECT_EQ(L, state2.get_state());
    lua_close(L);
}

TEST(state_view, move_assignment)
{
    lua_State *L = luaL_newstate();
    easylua::state_view state(L);
    easylua::state_view state2 = std::move(state);
    EXPECT_EQ(L, state2.get_state());
    lua_close(L);
}

TEST(state_view, get_global_bool)
{
    lua_State *L = luaL_newstate();
    lua_pushboolean(L, true);
    lua_setglobal(L, "test");

    easylua::state_view state(L);
    bool value = state["test"];
    EXPECT_EQ(true, value);
    lua_close(L);
}

TEST(state_view, get_global_int)
{
    lua_State *L = luaL_newstate();
    lua_pushinteger(L, 42);
    lua_setglobal(L, "test");

    easylua::state_view state(L);
    int value = state["test"];
    EXPECT_EQ(42, value);
    lua_close(L);
}

TEST(state_view, get_global_double)
{
    lua_State *L = luaL_newstate();
    lua_pushnumber(L, 3.14);
    lua_setglobal(L, "test");

    easylua::state_view state(L);
    double value = state["test"];
    EXPECT_EQ(3.14, value);
    lua_close(L);
}

TEST(state_view, get_global_string)
{
    lua_State *L = luaL_newstate();
    lua_pushstring(L, "hello");
    lua_setglobal(L, "test");

    easylua::state_view state(L);
    std::string value = state["test"];
    EXPECT_EQ("hello", value);
    lua_close(L);
}



