cmake_minimum_required(VERSION 3.24)
project(OSaSP_LABS C)

set(CMAKE_C_STANDARD 11)

add_compile_options(-W -Wall -Wno-unused-parameter -Wno-unused-variable -std=c11 -pedantic)

add_executable(LAB_1 LAB_1/main.c color_print.h)
add_executable(LAB_2 LAB_2/main.c LAB_2/func.c LAB_2/func.h color_print.h keyboard_input.h)
add_executable(LAB_2_Child LAB_2/child/main.c color_print.h)
add_executable(LAB_3_Parent
        LAB_3/src/protocol.h
        LAB_3/src/parent/main.c
        LAB_3/src/parent/helper.h
        LAB_3/src/parent/helper.c)
add_executable(LAB_3_Child
        LAB_3/src/protocol.h
        LAB_3/src/child/main.c
        LAB_3/src/child/helper.h
        LAB_3/src/child/helper.c)

add_executable(LAB_4
        LAB_4/src/main.c
        LAB_4/src/init_and_exit.h
        LAB_4/src/init_and_exit.c
        LAB_4/src/producer.h
        LAB_4/src/producer.c
        LAB_4/src/ipc.h
        LAB_4/src/ipc.c
        LAB_4/src/consumer.h
        LAB_4/src/consumer.c)

add_executable(LAB_5
        LAB_5/src/main.c
        LAB_5/src/init_and_exit.h
        LAB_5/src/init_and_exit.c
        LAB_5/src/producer.h
        LAB_5/src/producer.c
        LAB_5/src/ipc.h
        LAB_5/src/ipc.c
        LAB_5/src/consumer.h
        LAB_5/src/consumer.c)

add_executable(LAB_6_Generator
        LAB_6/src/generator/main.c)

target_include_directories(LAB_2 PRIVATE
        .)

target_include_directories(LAB_2_Child PRIVATE
        LAB_2/child
        .)

target_include_directories(LAB_3_Parent PRIVATE
        LAB_3/src
        .)

target_include_directories(LAB_3_Child PRIVATE
        LAB_3/src/child
        .)

target_include_directories(LAB_4 PRIVATE
        .)

target_include_directories(LAB_5 PRIVATE
        .)

target_include_directories(LAB_6_Generator PRIVATE
        .)