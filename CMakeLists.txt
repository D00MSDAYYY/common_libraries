cmake_minimum_required(VERSION 3.31.5)
project(common_libraries)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

####################### insert subdirectories here ##############################
add_subdirectory(script_engine)
add_subdirectory(actions_tree)
#################################################################################
