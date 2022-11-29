#include <easylua/stack.hpp>

#include <gtest/gtest.h>

class Stack : public ::testing::Test
{
public:
    Stack()
    {
        L = luaL_newstate();
        if (!L)
            throw std::runtime_error("Could not create Lua state");
    }

    ~Stack() { lua_close(L); }

protected:
    lua_State *L;
};

using namespace easylua;

TEST_F(Stack, get_index_zero_throws)
{
    EXPECT_THROW(stack::get<int>(L, 0), invalid_argument);
}

TEST_F(Stack, get_index_greater_than_one)
{
    lua_pushinteger(L, 42);
    lua_pushinteger(L, 43);
    lua_pushinteger(L, 44);

    EXPECT_EQ(stack::get<int>(L, 1), 42);
    EXPECT_EQ(stack::get<int>(L, 2), 43);
    EXPECT_EQ(stack::get<int>(L, 3), 44);
}

TEST_F(Stack, get_lua_integer)
{
    lua_pushinteger(L, 5);
    EXPECT_EQ(5, stack::get<lua_Integer>(L, 1));
}

TEST_F(Stack, get_lua_integer_throws_on_wrong_type)
{
    lua_pushstring(L, "hello");
    EXPECT_THROW(stack::get<lua_Integer>(L, 1), type_error);
}

TEST_F(Stack, get_integral_type)
{
    lua_pushinteger(L, 5);
    EXPECT_EQ(5, stack::get<int>(L, 1));
}

TEST_F(Stack, get_integral_type_throws_on_wrong_type)
{
    lua_pushstring(L, "hello");
    EXPECT_THROW(stack::get<int>(L, 1), type_error);
}

TEST_F(Stack, get_lua_number)
{
    lua_pushnumber(L, 5.5);
    EXPECT_EQ(5.5, stack::get<lua_Number>(L, 1));
}

TEST_F(Stack, get_lua_number_throws_on_wrong_type)
{
    lua_pushstring(L, "hello");
    EXPECT_THROW(stack::get<lua_Number>(L, 1), type_error);
}

TEST_F(Stack, get_floating_point_type)
{
    lua_pushnumber(L, 5.5);
    EXPECT_EQ(5.5, stack::get<double>(L, 1));
}

TEST_F(Stack, get_floating_point_type_throws_on_wrong_type)
{
    lua_pushstring(L, "hello");
    EXPECT_THROW(stack::get<float>(L, 1), type_error);
}

TEST_F(Stack, get_bool)
{
    lua_pushboolean(L, 1);
    EXPECT_TRUE(stack::get<bool>(L, 1));
}

TEST_F(Stack, get_bool_throws_on_wrong_type)
{
    lua_pushstring(L, "hello");
    EXPECT_THROW(stack::get<bool>(L, 1), type_error);
}

TEST_F(Stack, get_string)
{
    lua_pushstring(L, "Hello");
    EXPECT_EQ(std::string("Hello"), stack::get<std::string>(L, 1));
}

TEST_F(Stack, get_string_throws_on_wrong_type)
{
    lua_pushnumber(L, 5.5);
    EXPECT_THROW(stack::get<std::string>(L, 1), type_error);
}

TEST_F(Stack, get_string_view)
{
    lua_pushstring(L, "Hello");
    EXPECT_EQ(std::string_view("Hello"), stack::get<std::string_view>(L, 1));
}

TEST_F(Stack, get_string_view_throws_on_wrong_type)
{
    lua_pushnumber(L, 5.5);
    EXPECT_THROW(stack::get<std::string_view>(L, 1), type_error);
}

TEST_F(Stack, get_c_string)
{
    lua_pushstring(L, "Hello");
    EXPECT_STREQ("Hello", stack::get<const char *>(L, 1));
}

TEST_F(Stack, get_c_string_throws_on_wrong_type)
{
    lua_pushnumber(L, 5.5);
    EXPECT_THROW(stack::get<const char *>(L, 1), type_error);
}

TEST_F(Stack, push_multiple_values)
{
    stack::push(L, 42);
    stack::push(L, 43);
    stack::push(L, 44);

    EXPECT_EQ(lua_tointeger(L, 1), 42);
    EXPECT_EQ(lua_tointeger(L, 2), 43);
    EXPECT_EQ(lua_tointeger(L, 3), 44);
}

TEST_F(Stack, push_lua_integer)
{
    lua_Integer i = 5;
    stack::push(L, i);
    EXPECT_EQ(lua_tointeger(L, 1), 5);
}

TEST_F(Stack, push_integral_type)
{
    stack::push(L, 5);
    EXPECT_EQ(lua_tointeger(L, 1), 5);
}

TEST_F(Stack, push_lua_number)
{
    lua_Number n = 5.5;
    stack::push(L, n);
    EXPECT_EQ(lua_tonumber(L, 1), 5.5);
}

TEST_F(Stack, push_floating_point_type)
{
    stack::push(L, 5.5);
    EXPECT_EQ(lua_tonumber(L, 1), 5.5);
}

TEST_F(Stack, push_bool)
{
    stack::push(L, true);
    EXPECT_TRUE(lua_toboolean(L, 1));
}

TEST_F(Stack, push_string)
{
    stack::push(L, std::string("Hello"));
    EXPECT_EQ(std::string("Hello"), lua_tostring(L, 1));
}

TEST_F(Stack, push_string_view)
{
    stack::push(L, std::string_view("Hello"));
    EXPECT_EQ(std::string_view("Hello"), lua_tostring(L, 1));
}

TEST_F(Stack, push_c_string)
{
    stack::push(L, "Hello");
    EXPECT_STREQ("Hello", lua_tostring(L, 1));
}