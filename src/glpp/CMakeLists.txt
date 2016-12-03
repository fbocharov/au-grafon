cmake_minimum_required(VERSION 3.2)

project(glpp)

file(GLOB_RECURSE SOURCES "*.cpp" "*.h")

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${GL_LIBRARIES})
