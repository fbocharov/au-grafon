# CMake entry point
cmake_minimum_required(VERSION 3.2)
project(au-graphics)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(ASSIMP REQUIRED)

add_definitions(-DGLEW_STATIC)

set(GL_LIBRARIES
	${OPENGL_gl_LIBRARY}
	${GLEW_LIBRARIES}
	${ASSIMP_LIBRARIES}
	glfw
)

add_subdirectory(src)
