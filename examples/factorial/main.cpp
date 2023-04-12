#include <easylua/easylua.hpp>

#include <iostream>

static constexpr const char *factorial_lua = R"(
--- the classic recursive example:
function factorial(n)
if n == 0 then
   return 1
else
   return n * factorial(n-1)
end
end

-- and its tail recursive variant
fact = function (n)
  local f
  f = function (m,a)
      if m == 0 then return a end -- if
      return f(m-1,m*a) end -- function
  return f(n,1) end -- function
)";

int main(int argc, char **argv)
{
    easylua::state state;
    auto load_result = state.run(factorial_lua);
    if (!load_result)
    {
        std::cout << "Error: " << load_result.get_error_message() << std::endl;
        return 1;
    }

    easylua::safe_function_reference factorial = state["factorial"];
    int factorial_result = factorial(5);
    std::cout << "factorial(5) = " << factorial_result << std::endl;

    easylua::safe_function_reference fact = state["fact"];
    factorial_result = fact(10);
    std::cout << "fact(10) = " << factorial_result << std::endl;

    return 0;
}