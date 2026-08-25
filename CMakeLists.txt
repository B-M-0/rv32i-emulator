cmake_minimum_required(VERSION 3.16)
project(rv32i_emu CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(emu src/decode.cpp)
target_include_directories(emu PUBLIC include)

add_executable(rv32i src/main.cpp)
target_link_libraries(rv32i PRIVATE emu)

enable_testing()
add_executable(test_decode tests/test_decode.cpp)
target_link_libraries(test_decode PRIVATE emu)
add_test(NAME decode COMMAND test_decode)

