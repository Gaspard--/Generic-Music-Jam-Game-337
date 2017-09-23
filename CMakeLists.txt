cmake_minimum_required(VERSION 2.6)

project(Generic-Music-Jam_Game-337)

execute_process(COMMAND python2.7 gl3w_gen.py WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(HEADER_DIRECTORY "include")
set(SOURCE_DIRECTORY "source")

if (WIN32)
  set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /std:c++14")
else()
  set(CMAKE_CXX_STANDARD 14)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
  set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -W -Wall -Wextra -Wfloat-conversion -g3 -O3")
endif(WIN32)

message("Falgs used: '${CMAKE_CXX_FLAGS}'")

file(GLOB_RECURSE SOURCES_FILE "${SOURCE_DIRECTORY}/*.cpp" "${SOURCE_DIRECTORY}/*.c")
file(GLOB_RECURSE HEADERS_FILE "${HEADER_DIRECTORY}/*.hpp")

find_package(Threads REQUIRED)
find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
# find_package(glfw3 REQUIRED)

include_directories(
  ${OPENGL_INCLUDE_DIRS}
  ${GLFW_INCLUDE_DIRS}
  ${HEADER_DIRECTORY}
)

add_executable(
  ${PROJECT_NAME}
  ${SOURCES_FILE}
  ${HEADERS_FILE}
)

target_link_libraries(
  ${PROJECT_NAME}
)

target_link_libraries(${PROJECT_NAME} ${OPENGL_gl_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${GLFW_STATIC_LIBRARIES})
target_link_libraries(${PROJECT_NAME})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})
