cmake_minimum_required(VERSION 3.31.5)
project(script_object)

########################### find packages here ##################################
find_package(Lua REQUIRED)

####################### insert subdirectories here ##############################

#################################################################################

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME} 
PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/script_object.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/script_object.cpp
)

target_include_directories(${PROJECT_NAME}
PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
	${LUA_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} 
PUBLIC
	${LUA_LIBRARIES}
	script_engine

)



