#include <easylua/state.hpp>

#include <gtest/gtest.h>

TEST(StateView, constructor_throws_on_null_state)
{
    EXPECT_THROW(easylua::StateView(nullptr), easylua::InvalidArgumentException);
}

TEST(StateView, constructor)
{
    lua_State *L = luaL_newstate();
    easylua::StateView state(L);
    EXPECT_EQ(L, state.GetLuaState());
    lua_close(L);
}

TEST(StateView, copy_constructor)
{
    lua_State *L = luaL_newstate();
    easylua::StateView state(L);
    easylua::StateView state2(state);
    EXPECT_EQ(L, state2.GetLuaState());
    lua_close(L);
}

TEST(StateView, copy_assignment)
{
    lua_State *L = luaL_newstate();
    easylua::StateView state(L);
    easylua::StateView state2 = state;
    EXPECT_EQ(L, state2.GetLuaState());
    lua_close(L);
}

TEST(StateView, move_constructor)
{
    lua_State *L = luaL_newstate();
    easylua::StateView state(L);
    easylua::StateView state2(std::move(state));
    EXPECT_EQ(L, state2.GetLuaState());
    lua_close(L);
}

TEST(StateView, move_assignment)
{
    lua_State *L = luaL_newstate();
    easylua::StateView state(L);
    easylua::StateView state2 = std::move(state);
    EXPECT_EQ(L, state2.GetLuaState());
    lua_close(L);
}