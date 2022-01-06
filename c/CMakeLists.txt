cmake_minimum_required(VERSION 3.21)
project(c C)

set(CMAKE_C_STANDARD 11)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(include)

file(GLOB_RECURSE SRC src/*.c)
file(GLOB_RECURSE INCLUDES include/*.h)

add_executable(c src/lz4c.c include/lz4c.h src/utils.c include/utils.h)
target_link_libraries(c ${CONAN_LIBS})