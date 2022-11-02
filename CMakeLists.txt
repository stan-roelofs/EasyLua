cmake_minimum_required(VERSION 3.10.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(LuaWrapper VERSION 0.1.0)

include(CTest)
enable_testing()

set (HEADEARS
    include/lua_wrapper/lua_wrapper.h
)

set(SOURCES
    src/lua_wrapper.cpp
)

find_package(Lua51 REQUIRED)
include_directories(${LUA_INCLUDE_DIR})

add_library(LuaWrapper ${SOURCES} ${HEADERS})
target_include_directories(LuaWrapper PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(LuaWrapper ${LUA_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

add_subdirectory(test)