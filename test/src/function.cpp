#include <easylua/function.hpp>

#include <gtest/gtest.h>

using namespace easylua;

TEST(unsafe_function_reference, constructor_throws_on_null_state)
{
    EXPECT_THROW(unsafe_function_reference(nullptr), invalid_argument);
}

TEST(unsafe_function_reference, call_throws_on_invalid_index)
{
    lua_State *L = luaL_newstate();
    lua_pushnumber(L, 1);
    EXPECT_THROW(unsafe_function_reference(L, -1)(), type_error);
    lua_close(L);
}

TEST(unsafe_function_reference, call)
{
    lua_State *L = luaL_newstate();
    ASSERT_EQ(0, luaL_dostring(L, "function f() a = 1; end"));
    ASSERT_EQ(LUA_TFUNCTION, lua_getglobal(L, "f"));

    easylua::unsafe_function_reference f(L);
    f();

    lua_getglobal(L, "a");
    ASSERT_EQ(LUA_TNUMBER, lua_type(L, -1));
    ASSERT_EQ(1, lua_tonumber(L, -1));

    lua_close(L);
}

TEST(unsafe_function_reference, call_with_argument)
{
    lua_State *L = luaL_newstate();
    ASSERT_EQ(0, luaL_dostring(L, "function f(a) b = a; end"));
    ASSERT_EQ(LUA_TFUNCTION, lua_getglobal(L, "f"));

    easylua::unsafe_function_reference f(L);
    f(42);

    lua_getglobal(L, "b");
    ASSERT_EQ(LUA_TNUMBER, lua_type(L, -1));
    ASSERT_EQ(42, lua_tonumber(L, -1));

    lua_close(L);
}

TEST(unsafe_function_reference, call_with_multiple_arguments)
{
    lua_State *L = luaL_newstate();
    ASSERT_EQ(0, luaL_dostring(L, "function f(a, b) c = a + b; end"));
    ASSERT_EQ(LUA_TFUNCTION, lua_getglobal(L, "f"));

    easylua::unsafe_function_reference f(L);
    f(1, 2);

    lua_getglobal(L, "c");
    ASSERT_EQ(LUA_TNUMBER, lua_type(L, -1));
    ASSERT_EQ(3, lua_tonumber(L, -1));

    lua_close(L);
}

TEST(unsafe_function_reference, call_with_no_return_throws_when_getting_return_value)
{
    lua_State *L = luaL_newstate();
    ASSERT_EQ(0, luaL_dostring(L, "function f() end"));
    ASSERT_EQ(LUA_TFUNCTION, lua_getglobal(L, "f"));

    easylua::unsafe_function_reference f(L);
    EXPECT_THROW({ int a = f(); }, runtime_error);

    lua_close(L);
}

TEST(unsafe_function_reference, call_with_return)
{
    lua_State *L = luaL_newstate();
    ASSERT_EQ(0, luaL_dostring(L, "function f() return 42; end"));
    ASSERT_EQ(LUA_TFUNCTION, lua_getglobal(L, "f"));

    easylua::unsafe_function_reference f(L);
    int result = f();

    ASSERT_EQ(42, result);

    lua_close(L);
}

TEST(unsafe_function_reference, call_with_multiple_returns)
{
    lua_State *L = luaL_newstate();
    ASSERT_EQ(0, luaL_dostring(L, "function f() return 1, \"abc\"; end"));
    ASSERT_EQ(LUA_TFUNCTION, lua_getglobal(L, "f"));

    easylua::unsafe_function_reference f(L);
    const std::tuple<int, std::string_view> result = f();

    ASSERT_EQ(1, std::get<0>(result));
    ASSERT_EQ("abc", std::get<1>(result));

    lua_close(L);
}

TEST(unsafe_function_reference, call_with_parameter_and_return)
{
    lua_State *L = luaL_newstate();
    ASSERT_EQ(0,
              luaL_dostring(L, "function f(a) return a + 1, a + 2, a + 3; end"));
    ASSERT_EQ(LUA_TFUNCTION, lua_getglobal(L, "f"));

    easylua::unsafe_function_reference f(L);
    std::tuple<short, int, long> result = f(42);

    ASSERT_EQ(43, std::get<0>(result));
    ASSERT_EQ(44, std::get<1>(result));
    ASSERT_EQ(45, std::get<2>(result));

    lua_close(L);
}