#include <easylua/easylua.h>

#include <gtest/gtest.h>

#include <sstream>

using namespace easylua;

class Call : public ::testing::Test
{
public:
    Call()
    {
    }

protected:
    void LoadString(const char *str)
    {
        ASSERT_EQ(0, luaL_dostring(context_.GetRawState(), str));
    }

    LuaContext context_;
};

TEST_F(Call, no_parameters_no_return)
{
    LoadString("function test() end");
    const std::tuple<> result = context_.Call("test");
}

TEST_F(Call, single_parameter)
{
    LoadString("function test(a) parameter1 = a; end");
    context_.Call("test", 5);
    context_.GetGlobal("parameter1");
    EXPECT_EQ(5, context_.Read<integer>());
}

TEST_F(Call, single_return)
{
    LoadString("function test() return 5; end");
    const auto result = context_.Call<integer>("test");
    EXPECT_EQ(5, std::get<0>(result));
}

TEST_F(Call, multiple_parameters)
{
    LoadString("function test(a, b) parameter1 = a; parameter2 = b; end");
    const auto result = context_.Call("test", 1, " b ");
    context_.GetGlobal("parameter1");
    EXPECT_EQ(1, context_.Read<integer>());
    context_.GetGlobal("parameter2");
    EXPECT_STREQ(" b ", context_.Read<string>());
}

TEST_F(Call, multiple_returns)
{
    LoadString("function test() return 5, 3.0, true; end");
    const auto result = context_.Call<integer, number, boolean>("test");
    EXPECT_EQ(5, std::get<0>(result));
    EXPECT_FLOAT_EQ(3.0f, std::get<1>(result));
    EXPECT_EQ(true, std::get<2>(result));
}

TEST_F(Call, multiple_parameters_multiple_returns)
{
    LoadString("function test(a, b) return a,b end");
    const auto result = context_.Call<integer, boolean>("test", 123, false);
    EXPECT_EQ(123, std::get<0>(result));
    EXPECT_EQ(false, std::get<1>(result));
}