cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(dual_memory_game C CXX ASM)
pico_sdk_init()

add_executable(dual_memory_game
    main.c
    buffer/buffer.c
    engine/word/word_engine.c
    engine/led/led_engine.c
    peripherals/buttons/buttons.c
    peripherals/leds/leds.c
    peripherals/keyboard/keyboard.c
    peripherals/display/display.c
)

target_include_directories(dual_memory_game PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(dual_memory_game
    pico_stdlib
    pico_multicore 
    pico_rand
    hardware_i2c
)

pico_enable_stdio_usb(dual_memory_game 1)
pico_enable_stdio_uart(dual_memory_game 0)

pico_add_extra_outputs(dual_memory_game)