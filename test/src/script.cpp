#include <easylua/script.hpp>

#include <gtest/gtest.h>

class Script : public ::testing::Test
{
public:
    Script()
    {
        L = luaL_newstate();
        if (!L)
            throw std::runtime_error("Could not create Lua state");
    }

    ~Script() { lua_close(L); }

protected:
    lua_State *L;
};

using namespace easylua;

TEST_F(Script, load_string_succeeds)
{
    ASSERT_EQ(script::load_result::result::ok,
              script::load_string(L, "function test() end").get_result());
}

TEST_F(Script, load_string_does_not_execute)
{
    ASSERT_EQ(script::load_result::result::ok,
              script::load_string(L, "test = 5").get_result());
    lua_getglobal(L, "test");
    ASSERT_EQ(LUA_TNIL, lua_type(L, -1));
}

TEST_F(Script, load_string_fails_on_syntax_error)
{
    ASSERT_EQ(script::load_result::result::syntax_error,
              script::load_string(L, "function test()").get_result());
}

TEST_F(Script, run_string_succeeds)
{
    ASSERT_TRUE(script::run_string(L, "test = 5"));
    lua_getglobal(L, "test");
    ASSERT_EQ(LUA_TNUMBER, lua_type(L, -1));
    ASSERT_EQ(5, lua_tonumber(L, -1));
}