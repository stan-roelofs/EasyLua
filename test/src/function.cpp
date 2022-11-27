#include <easylua/function.hpp>

#include <gtest/gtest.h>

using namespace easylua;

TEST(Function, constructor_throws_on_null_state)
{
    EXPECT_THROW(Function(nullptr), InvalidArgumentException);
}

TEST(Function, call)
{
    lua_State *L = luaL_newstate();
    ASSERT_EQ(0, luaL_dostring(L, "function f() a = 1; end"));
    ASSERT_EQ(LUA_TFUNCTION, lua_getglobal(L, "f"));

    easylua::Function f(L);
    f();

    lua_getglobal(L, "a");
    ASSERT_EQ(LUA_TNUMBER, lua_type(L, -1));
    ASSERT_EQ(1, lua_tonumber(L, -1));

    lua_close(L);
}

TEST(Function, call_with_argument)
{
    lua_State *L = luaL_newstate();
    ASSERT_EQ(0, luaL_dostring(L, "function f(a) b = a; end"));
    ASSERT_EQ(LUA_TFUNCTION, lua_getglobal(L, "f"));

    easylua::Function f(L);
    f(42);

    lua_getglobal(L, "b");
    ASSERT_EQ(LUA_TNUMBER, lua_type(L, -1));
    ASSERT_EQ(42, lua_tonumber(L, -1));

    lua_close(L);
}

TEST(Function, call_with_multiple_arguments)
{
    lua_State *L = luaL_newstate();
    ASSERT_EQ(0, luaL_dostring(L, "function f(a, b) c = a + b; end"));
    ASSERT_EQ(LUA_TFUNCTION, lua_getglobal(L, "f"));

    easylua::Function f(L);
    f(1, 2);

    lua_getglobal(L, "c");
    ASSERT_EQ(LUA_TNUMBER, lua_type(L, -1));
    ASSERT_EQ(3, lua_tonumber(L, -1));

    lua_close(L);
}

TEST(Function, call_with_return)
{
    lua_State *L = luaL_newstate();
    ASSERT_EQ(0, luaL_dostring(L, "function f() return 42; end"));
    ASSERT_EQ(LUA_TFUNCTION, lua_getglobal(L, "f"));

    easylua::Function f(L);
    int result = f();

    ASSERT_EQ(42, result);

    lua_close(L);
}