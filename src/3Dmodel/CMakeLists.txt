cmake_minimum_required(VERSION 3.2)

project(3Dmodel)

file(GLOB_RECURSE SOURCES "*.cpp" "*.h" "*.frag" "*.vert")

include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
	${GL_LIBRARIES}
	glpp
	SOIL
)
