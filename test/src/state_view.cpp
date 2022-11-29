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