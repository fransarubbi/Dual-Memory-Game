cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(pico_blink C CXX ASM)

pico_sdk_init()

add_executable(pico_blink
    main.c
)

target_link_libraries(pico_blink
    pico_stdlib
)

pico_add_extra_outputs(pico_blink)
