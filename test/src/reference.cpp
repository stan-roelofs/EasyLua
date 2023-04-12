#include <easylua/reference.hpp>

#include <gtest/gtest.h>

struct mock_unsafe_reference : easylua::unsafe_reference
{
    mock_unsafe_reference(lua_State *state, int index) : unsafe_reference(state, index) {}
};

TEST(unsafe_reference, constructor_throws_on_null_state)
{
    EXPECT_THROW(mock_unsafe_reference(nullptr, 0), easylua::invalid_argument);
}

TEST(unsafe_reference, constructor_throws_on_zero_index)
{
    lua_State *L = luaL_newstate();
    EXPECT_THROW(mock_unsafe_reference(L, 0), easylua::invalid_argument);
    lua_close(L);
}

TEST(unsafe_reference, constructor_throws_on_invalid_index)
{
    lua_State *L = luaL_newstate();
    EXPECT_THROW(mock_unsafe_reference(L, 1), easylua::invalid_argument);
    lua_close(L);
}

TEST(unsafe_reference, push)
{
    lua_State *L = luaL_newstate();
    lua_pushnumber(L, 5);
    mock_unsafe_reference ref(L, 1);
    ref.push();
    ASSERT_EQ(LUA_TNUMBER, lua_type(L, -1));
    ASSERT_EQ(5, lua_tonumber(L, -1));
    lua_close(L);
}

struct mock_safe_reference : easylua::safe_reference
{
    mock_safe_reference(lua_State *state, int index) : safe_reference(state, index) {}

    int &reference()
    {
        return reference_;
    }
};

TEST(safe_reference, constructor_throws_on_null_state)
{
    EXPECT_THROW(mock_safe_reference(nullptr, 0), easylua::invalid_argument);
}

TEST(safe_reference, constructor_throws_on_zero_index)
{
    lua_State *L = luaL_newstate();
    EXPECT_THROW(mock_safe_reference(L, 0), easylua::invalid_argument);
    lua_close(L);
}

TEST(safe_reference, constructor_throws_on_invalid_index)
{
    lua_State *L = luaL_newstate();
    EXPECT_THROW(mock_safe_reference(L, 1), easylua::invalid_argument);
    lua_close(L);
}

// TODO copy, move

TEST(safe_reference, push)
{
    lua_State *L = luaL_newstate();
    lua_pushnumber(L, 5);
    {
        mock_safe_reference ref(L, 1);
        ref.push();
        // ref.push();
        ASSERT_EQ(LUA_TNUMBER, lua_type(L, -1));
        ASSERT_EQ(5, lua_tonumber(L, -1));
    }
    lua_close(L);
}

TEST(safe_reference, push_throws_with_invalid_reference)
{
    lua_State *L = luaL_newstate();
    lua_pushnumber(L, 5);
    {
        mock_safe_reference ref(L, 1);
        ref.reference() = LUA_NOREF;
        EXPECT_THROW(ref.push(), easylua::runtime_error);
    }
    lua_close(L);
}