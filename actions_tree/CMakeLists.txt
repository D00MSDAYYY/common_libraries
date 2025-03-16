cmake_minimum_required(VERSION 3.31.5)
project(actions_tree LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

####################### insert subdirectories here ##############################

#################################################################################

add_library(${PROJECT_NAME}  INTERFACE)

target_sources(${PROJECT_NAME} 
PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/actions_tree.hpp
	# ${CMAKE_CURRENT_SOURCE_DIR}/actions_tree.cpp
)

target_include_directories(${PROJECT_NAME}
INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}
)