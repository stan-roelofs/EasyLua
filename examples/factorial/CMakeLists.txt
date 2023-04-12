project(factorial_example)

find_package(Lua REQUIRED)

add_executable ( factorial_example main.cpp )
target_include_directories(factorial_example PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries ( factorial_example lua )