cmake_minimum_required(VERSION 3.6)
project(bigo)

set(CMAKE_C_STANDARD 99)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)
set(SOURCE_FILES src/max.c)
set(CMAKE_C_FLAGS -O3)

add_executable(bigo ${SOURCE_FILES})
add_executable(tp2 src/tp2.c)
add_executable(fib src/fib.c)
