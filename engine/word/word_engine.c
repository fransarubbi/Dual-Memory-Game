#include "engine/word/word_engine.h"
#include "pico/stdlib.h"
#include "peripherals/keyboard/keyboard.h"

void word_game() {
    init_keyboard();
    while (1) {
        update_fsm_keyboard();
        sleep_ms(25);
    }
    
}