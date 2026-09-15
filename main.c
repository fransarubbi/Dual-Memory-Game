#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "engine/led/led_engine.h"
#include "engine/word/word_engine.h"

int main(void) {
    // inicializar I/O general de la placa
    stdio_init_all();
    
    // activar core 1
    multicore_launch_core1(word_game);

    // usar core 0 para el juego de leds
    led_game();

    return 0;
}

// cmake -B build  cuando se modifica cmake
// cmake --build build  compilar