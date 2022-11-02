#include <lua_wrapper/lua_wrapper.h>

#include <gtest/gtest.h>

TEST(test, test_call)
{
    lua_wrapper::LuaContext l;
    auto got = l.Call("test", 1, "b", 3.f);
    auto got2 = l.Call<int, const char *>("test", 1, "b", 3.f);

    std::cout << std::get<0>(got2) << " " << std::get<1>(got2) << std::endl;
}