cmake_minimum_required(VERSION 3.31.5)
project(scripting)

########################### find packages here ##################################
# find_package(Lua REQUIRED) # don't use this because i use static_lua version 
####################### insert subdirectories here ##############################
add_subdirectory(libraries)

#################################################################################

add_library(${PROJECT_NAME} INTERFACE)

target_sources(${PROJECT_NAME} 
INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.hpp
)

target_include_directories(${PROJECT_NAME}
INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}
	${LUA_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} 
INTERFACE
	lua::lib
	sol2::sol2
)



