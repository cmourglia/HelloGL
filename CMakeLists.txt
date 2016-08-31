cmake_minimum_required(VERSION 3.0.2)
project(hello_gl)

# Setup externals
set(GLFW_ROOT "${CMAKE_SOURCE_DIR}/external/glfw")
set(GL3W_ROOT "${CMAKE_SOURCE_DIR}/external/gl3w")

set(GL3W_ARB "${GL3W_ROOT}/include/GL/glcorearb.h")
set(GL3W_INC "${GL3W_ROOT}/include/GL/gl3w.h")
set(GL3W_SRC "${GL3W_ROOT}/src/gl3w.c")

if (NOT EXISTS ${GL3W_ROOT} OR NOT EXISTS ${GLFW_ROOT})
	message(STATUS "Running git submodule update...")
	execute_process(COMMAND git submodule update)
endif()

if (NOT ((EXISTS ${GL3W_ARB}) AND (EXISTS ${GL3W_INC}) AND (EXISTS ${GL3W_SRC})))
	message(STATUS "Running python gl3w_gen.py")
	execute_process(COMMAND python gl3w_gen.py
					WORKING_DIRECTORY ${GL3W_ROOT})
endif()

# Setup c++ flags
if (UNIX)
    set(CMAKE_CXX_STANDARD 14) # Require c++14
else()

endif()

# Compile GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${GLFW_ROOT})

find_package(OpenGL REQUIRED)

include_directories(
	${GLFW_ROOT}/include
	${GL3W_ROOT}/include
	${OPENGL_INCLUDE_DIR}
)

add_executable(hello_gl src/main.cpp ${GL3W_SRC})
target_link_libraries(
	hello_gl
	${OPENGL_gl_LIBRARY}
	glfw
)
