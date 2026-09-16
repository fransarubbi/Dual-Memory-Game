#include <stdio.h>
#include "engine/word/word_engine.h"
#include "pico/stdlib.h"
#include "pico/rand.h"
#include "peripherals/keyboard/keyboard.h"
#include "peripherals/display/display.h"
#include "buffer/buffer.h"


typedef enum {
    INIT,
    GENERATE,
    REPRODUCE,
    ANSWER,
    COMPARE,
    WAIT_FOR_RESET
} WordGameState;

static WordGameState state = INIT;
static uint8_t level = 1;  // de 1 a 10
static char msg[17];

static Event random_letter() {
    uint32_t num = (get_rand_32() % 16);
    static const Event map[16] = {
        ONE,    TWO,    THREE,  LETTER_A,
        FOUR,   FIVE,   SIX,    LETTER_B,
        SEVEN,  EIGHT,  NINE,   LETTER_C,
        OK,     ZERO,     OK,   LETTER_D
    };
    if (num == 12 || num == 14) return map[num + 1];
    return map[num];
}

static void generate_sequence() {
    for (uint8_t i = 0; i < level; i++) {
        Event event = random_letter();
        insert_buffer(&buffer_key, event, ORIGINAL);
    }
}

static void reproduce_sequence() {
    for (uint8_t i = 0; i < level; i++) {
        Event event = get_event(&buffer_key, ORIGINAL, i);
        char character = convert_to_char(event);
        lcd_set_cursor(1, 0);
        lcd_print("                ");
        sleep_ms(400);
        lcd_set_cursor(1, 0);
        lcd_send_char(character);
        sleep_ms(400);
        lcd_set_cursor(1, 0);
        lcd_print("                ");
    }
}

static void victory() {
    for (uint8_t i = 0; i < 20; i++) {
        lcd_set_cursor(1, 0);
        lcd_print("Ganaste!");
    }
}

static void defeat() {
    for (uint8_t i = 0; i < 20; i++) {
        lcd_set_cursor(1, 0);
        lcd_print("Perdiste :(");
    }
}

static void fsm() {
    switch (state) {
        case INIT:
            if (is_ok(&buffer_key)) {
                state = GENERATE;
            }
        break;

        case GENERATE:
            snprintf(msg, sizeof(msg), "Nivel: %u", level);
            lcd_set_cursor(0, 0); 
            lcd_print(msg);
            clean_buffer(&buffer_key);
            generate_sequence();
            state = REPRODUCE;
            break;

        case REPRODUCE:
            reproduce_sequence();
            clean_inserted(&buffer_key);
            state = ANSWER;
        break;

        case ANSWER:
            if (is_ok(&buffer_key)) {
                state = COMPARE;
            }
        break;

        case COMPARE:
            bool answ = compare_buffer(&buffer_key);
            if (answ) {
                if (level == 10) {
                    victory();
                    state = WAIT_FOR_RESET;
                } else {
                    level++;
                    state = GENERATE;
                }
            } else {
                defeat();
                state = WAIT_FOR_RESET;
            }
        break;

        case WAIT_FOR_RESET:
            if (is_ok(&buffer_key)) {
                clean_buffer(&buffer_key);
                lcd_set_cursor(1, 0);
                lcd_print("                ");
                level = 1;
                state = INIT;
            }
        break;

        default:
        break;
    }
}


void word_game() {
    clean_buffer(&buffer_key);
    init_keyboard();
    lcd_init(SDA, SCL);

    while (1) {
        fsm();
        fsm_keyboard();
        sleep_ms(25);
    }
}