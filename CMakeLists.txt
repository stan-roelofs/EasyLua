cmake_minimum_required(VERSION 3.10.0)

project(EasyLua VERSION 0.1.0
    DESCRIPTION "A header-only library C++ wrapper for Lua"
    HOMEPAGE_URL "https://github.com/stan-roelofs/EasyLua"
    LANGUAGES CXX
)

add_library(EasyLua INTERFACE)
target_include_directories(EasyLua INTERFACE include)
target_compile_features(EasyLua INTERFACE cxx_std_17)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

add_subdirectory(test)