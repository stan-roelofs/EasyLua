cmake_minimum_required(VERSION 3.10.0)

project(EasyLua VERSION 0.1.0
    DESCRIPTION "A header-only C++ wrapper for Lua"
    HOMEPAGE_URL "https://github.com/stan-roelofs/EasyLua"
    LANGUAGES CXX
)

add_library(EasyLua INTERFACE)
target_include_directories(EasyLua INTERFACE include)
target_compile_features(EasyLua INTERFACE cxx_std_17)

include(CTest)
enable_testing()

add_test(
    NAME EasyLuaTest
    COMMAND $<TARGET_FILE:EasyLuaTest>
)

add_subdirectory(test)
add_subdirectory(examples)