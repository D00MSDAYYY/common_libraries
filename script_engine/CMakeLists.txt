cmake_minimum_required(VERSION 3.31.5)
project(script_engine)

########################### find packages here ##################################
find_package(Lua REQUIRED)
####################### insert subdirectories here ##############################
add_subdirectory(sol2)
add_subdirectory(script_object)
#################################################################################

add_library(${PROJECT_NAME} INTERFACE)

target_sources(${PROJECT_NAME} 
PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/script_engine.hpp
)

target_include_directories(${PROJECT_NAME}
INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}
	${LUA_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} 
INTERFACE
	${LUA_LIBRARIES}
	sol2
)



