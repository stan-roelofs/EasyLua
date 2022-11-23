#include <easylua/state.hpp>

#include <gtest/gtest.h>

TEST(State, constructor)
{
    easylua::State state;
    EXPECT_NE(nullptr, state.GetLuaState());
}

TEST(State, move_constructor)
{
    easylua::State state;
    easylua::State state2(std::move(state));
    EXPECT_NE(nullptr, state2.GetLuaState());
}

TEST(State, move_assignment)
{
    easylua::State state;
    easylua::State state2 = std::move(state);
    EXPECT_NE(nullptr, state2.GetLuaState());
}
