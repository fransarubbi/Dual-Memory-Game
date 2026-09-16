#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "pico/rand.h"
#include "peripherals/leds/leds.h"
#include "peripherals/buttons/buttons.h"
#include "led_engine.h"
#include "buffer/buffer.h"


typedef enum {
    INIT,
    GENERATE,
    REPRODUCE,
    ANSWER,
    COMPARE,
    WAIT_FOR_RESET
} LedGameState;

static LedGameState state = INIT;
static uint8_t level = 1;  // de 1 a 10


static Event random_led() {
    uint32_t num = (get_rand_32() % 4);
    if (num == 0) {
        Event e = RED_LED;
        return e;
    } 
    if (num == 1) {
        Event e = BLUE_LED;
        return e;
    } 
    if (num == 2) {
        Event e = GREEN_LED;
        return e;
    } 
    if (num == 3) {
        Event e = YELLOW_LED;
        return e;
    }
    
}

static void generate_sequence() {
    for (uint8_t i = 0; i < level; i++) {
        Event event = random_led();
        insert_buffer(&buffer_leds, event, ORIGINAL);
    }
}

static void reproduce_sequence() {
    for (uint8_t i = 0; i < level; i++) {
        Event event = get_event(&buffer_leds, ORIGINAL, i);
        on_led(event);
        sleep_ms(REPRODUCE_TIME);
        off_led(event);
        sleep_ms(REPRODUCE_TIME);
    }
}

static void victory() {
    for (uint8_t i = 0; i < 20; i++) {
        Event event = GREEN_LED;
        on_led(event);
        sleep_ms(WIN_OR_LOSE);
        off_led(event);
        sleep_ms(WIN_OR_LOSE);
    }
}

static void defeat() {
    for (uint8_t i = 0; i < 20; i++) {
        Event event = RED_LED;
        on_led(event);
        sleep_ms(WIN_OR_LOSE);
        off_led(event);
        sleep_ms(WIN_OR_LOSE);
    }
}

static void fsm() {
    switch (state) {
        case INIT:
            if (is_ok(&buffer_leds)) {
                state = GENERATE;
            }
        break;

        case GENERATE:
            clean_buffer(&buffer_leds);
            generate_sequence();
            state = REPRODUCE;
            break;

        case REPRODUCE:
            reproduce_sequence();
            clean_inserted(&buffer_leds);
            state = ANSWER;
        break;

        case ANSWER:
            if (is_ok(&buffer_leds)) {
                state = COMPARE;
            }
        break;

        case COMPARE:
            bool answ = compare_buffer(&buffer_leds);
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
            if (is_ok(&buffer_leds)) {
                clean_buffer(&buffer_leds);
                level = 1;
                state = INIT;
            }
        break;

        default:
        break;
    }
}

void led_game() {
    clean_buffer(&buffer_leds);
    init_leds();
    init_buttons();

    while (1) {
        fsm();
        read_buttons();
        sleep_ms(SLEEP); 
    }
}