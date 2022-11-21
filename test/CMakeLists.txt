cmake_minimum_required(VERSION 3.18.0)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        15460959cbbfa20e66ef0b5ab497367e47fc0a04 # release-1.12.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(SOURCES
    src/call.cpp
)

find_package(Lua REQUIRED)

add_executable(EasyLuaTest ${SOURCES})
target_link_libraries(EasyLuaTest lua gtest_main)
target_include_directories(EasyLuaTest PRIVATE ${CMAKE_SOURCE_DIR}/include)

include(GoogleTest)
gtest_discover_tests(EasyLuaTest)