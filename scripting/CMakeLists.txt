cmake_minimum_required(VERSION 3.31.5)
project(scripting)

########################### find packages here ##################################
find_package(Lua REQUIRED)
####################### insert subdirectories here ##############################
add_subdirectory(sol2)
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
	${LUA_LIBRARIES}
	sol2::sol2
)



