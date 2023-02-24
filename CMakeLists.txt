cmake_minimum_required(VERSION 3.24)
project(OSaSP_LABS C)

set(CMAKE_C_STANDARD 11)

add_compile_options(-W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 -pedantic)

add_executable(LAB_1 LAB_1/main.c color_print.h)
add_executable(LAB_2 LAB_02/main.c LAB_02/func.c LAB_02/func.h color_print.h keyboard_input.h)
add_executable(LAB_2_Child LAB_02/child/main.c color_print.h)

target_include_directories(LAB_2 PRIVATE
        .)

target_include_directories(LAB_2_Child PRIVATE
        LAB_02/child
        .)
