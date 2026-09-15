#include <stdio.h>
#include "peripherals/leds/leds.h"
#include "pico/stdlib.h"


/*
rojo - GP28
azul - GP22
verde - GP26
amarillo - GP27
*/


typedef struct {
    unsigned int pin;
} Leds;


static const Leds leds[4] = {
    {28},
    {22},
    {26},
    {27}
};

void init_leds() {
    for (uint8_t i = 0; i < 4; i++) {
        gpio_init(leds[i].pin);
        gpio_set_dir(leds[i].pin, GPIO_OUT);
    }
}

void on_led(type_led type) {
    gpio_put(leds[type].pin, 1);
}

void off_led(type_led type) {
    gpio_put(leds[type].pin, 0);
}