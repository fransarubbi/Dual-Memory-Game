#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "pico/rand.h"
#include "peripherals/leds/leds.h"
#include "peripherals/buttons/buttons.h"
#include "buffer/buffer.h"


typedef enum {
    GENERATE,
    REPRODUCE,
    ANSWER,
    COMPARE,
    WAIT_FOR_RESET
} LedGameState;

static LedGameState state = GENERATE;
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
        sleep_ms(400);
        off_led(event);
        sleep_ms(400);
    }
}

static void victory() {
    for (uint8_t i = 0; i < 20; i++) {
        Event event = GREEN_LED;
        on_led(event);
        sleep_ms(300);
        off_led(event);
        sleep_ms(300);
    }
}

static void defeat() {
    for (uint8_t i = 0; i < 20; i++) {
        Event event = RED_LED;
        on_led(event);
        sleep_ms(300);
        off_led(event);
        sleep_ms(300);
    }
}

static void fsm() {
    switch (state) {
        case GENERATE:
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
                victory();
            } else {
                defeat();
            }
        break;

        case WAIT_FOR_RESET:
            if (is_ok(&buffer_leds)) {
                clean_buffer(&buffer_leds);
                state = GENERATE;
                sleep_ms(1000); 
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
        sleep_ms(500); 
    }
}