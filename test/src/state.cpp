#include <easylua/state.hpp>

#include <gtest/gtest.h>

TEST(state, constructor)
{
    easylua::state state;
    EXPECT_NE(nullptr, state.get_state());
}

TEST(state, move_constructor)
{
    easylua::state state;
    easylua::state state2(std::move(state));
    EXPECT_NE(nullptr, state2.get_state());
}

TEST(state, move_assignment)
{
    easylua::state state;
    easylua::state state2 = std::move(state);
    EXPECT_NE(nullptr, state2.get_state());
}
