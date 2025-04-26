cmake_minimum_required(VERSION 3.31.5)
project(libraries)

include(FetchContent)

set(SOL2_DEP_NAME "sol2")
set(SOL2_REMOTE_REPO "https://github.com/ThePhD/sol2.git")
set(SOL2_TAG "main")
set(SOL2_DEP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${SOL2_DEP_NAME}")

FetchContent_Declare(
    ${SOL2_DEP_NAME}
    GIT_REPOSITORY ${SOL2_REMOTE_REPO}
    GIT_TAG ${SOL2_TAG}
    GIT_SHALLOW TRUE
    SOURCE_DIR "${SOL2_DEP_DIR}" 
)

FetchContent_MakeAvailable(${SOL2_DEP_NAME})


set(STATIC_LUA_DEP_NAME "static_lua")
set(STATIC_LUA_REMOTE_REPO "https://github.com/lubgr/lua-cmake.git")
set(STATIC_LUA_TAG "master")
set(STATIC_LUA_DEP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${STATIC_LUA_DEP_NAME}")

FetchContent_Declare(
    ${STATIC_LUA_DEP_NAME}
    GIT_REPOSITORY ${STATIC_LUA_REMOTE_REPO}
    GIT_TAG ${STATIC_LUA_TAG}
    GIT_SHALLOW TRUE
    SOURCE_DIR "${STATIC_LUA_DEP_DIR}" 
)

FetchContent_MakeAvailable(${STATIC_LUA_DEP_NAME})