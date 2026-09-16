#include "engine/word/word_engine.h"
#include "pico/stdlib.h"
#include "peripherals/keyboard/keyboard.h"
#include "peripherals/display/display.h"

void word_game() {
    init_keyboard();
    lcd_init(SDA, SCL);
    lcd_set_cursor(0, 0);
    lcd_print("Hola Franco");
    while (1) {
        update_fsm_keyboard();
        sleep_ms(25);
    }
    
}