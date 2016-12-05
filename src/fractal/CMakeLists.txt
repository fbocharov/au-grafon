cmake_minimum_required(VERSION 3.2)

project(fractal)

file(GLOB_RECURSE SOURCES "*.cpp" "*.h" "*.vert" "*.frag")

include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
	${GL_LIBRARIES}
	glpp
	SOIL
)
