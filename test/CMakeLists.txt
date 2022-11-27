cmake_minimum_required(VERSION 3.18.0)

set(SOURCES
    src/function.cpp
    src/script.cpp
    src/stack.cpp
    src/state.cpp
    src/state_view.cpp
)

add_executable(EasyLuaTest ${SOURCES})

find_package(Lua REQUIRED)
find_package(GTest REQUIRED)

target_link_libraries(EasyLuaTest lua gtest gtest_main)
target_include_directories(EasyLuaTest PRIVATE ${CMAKE_SOURCE_DIR}/include ${GTEST_INCLUDE_DIRS})

include(GoogleTest)
gtest_discover_tests(EasyLuaTest)