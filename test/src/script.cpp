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
    ASSERT_EQ(script::LoadResult::kOK,
              script::FromString(L, "function test() end"));
}

TEST_F(Script, load_string_fails_on_syntax_error)
{
    ASSERT_EQ(script::LoadResult::kSyntaxError,
              script::FromString(L, "function test()"));
}