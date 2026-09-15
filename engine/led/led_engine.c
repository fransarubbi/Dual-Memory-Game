#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "pico/rand.h"
#include "leds/leds.h"
#include "buffer/buffer.h"


static uint8_t level = 1;  // de 1 a 10
Buffer buffer_leds;


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


void generate_sequence() {
    switch (level) {
    case 1:
        Event event = random_led();
        insert_buffer(&buffer_leds, event, ORIGINAL);
        break;
    
    case 2:
        
    default:
        break;
    } 
}


void led_game() {
    
}