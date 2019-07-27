cmake_minimum_required(VERSION 3.14)
project(hello_gl)

set(CMAKE_CXX_STANDARD 20)

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
	message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
	file(DOWNLOAD "https://github.com/conan-io/cmake-conan/raw/v0.14/conan.cmake"
	              "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_run(
	REQUIRES
		glad/0.1.29@bincrafters/stable
		glfw/3.3@bincrafters/stable
	BASIC_SETUP
		CMAKE_TARGETS
	BUILD
		missing
	OPTIONS
		glad:profile=core
		glad:api_type=gl
		glad:api_version=4.5)

find_package(OpenGL REQUIRED)

add_executable(hello_gl src/main.cpp)
target_link_libraries(hello_gl OpenGL::GL CONAN_PKG::glad CONAN_PKG::glfw)
