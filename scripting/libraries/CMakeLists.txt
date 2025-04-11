cmake_minimum_required(VERSION 3.31.5)
project(libraries)

include(FetchContent)

set(DEP_NAME "sol2")
set(REMOTE_REPO "https://github.com/ThePhD/sol2.git")
set(TAG "main")
set(DEP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${DEP_NAME}")

FetchContent_Declare(
    ${DEP_NAME}
    GIT_REPOSITORY ${REMOTE_REPO}
    GIT_TAG ${TAG}
    GIT_SHALLOW TRUE
    SOURCE_DIR "${DEP_DIR}" 
)

FetchContent_MakeAvailable(${DEP_NAME})