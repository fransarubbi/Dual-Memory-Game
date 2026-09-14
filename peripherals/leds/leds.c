#include "peripherals/leds/leds.h"
#include "pico/stdlib.h"


/*
azul - GP22
verde - GP26
amarillo - GP27
rojo - GP28
*/


typedef struct {
    unsigned int pin;
    gpio_dir mode;
} Leds;


static const Leds leds[4] = {
    {28, GPIO_OUT},
    {22, GPIO_OUT},
    {27, GPIO_OUT},
    {26, GPIO_OUT},
};

void init_leds() {
    for (uint8_t i = 0; i < 4; i++) {
        gpio_init(leds[i].pin);
    }
}

void on_led(type_led) {
    gpio_put(leds[type_led].pin, 1);
}

void off_led(type_led) {
    gpio_put(leds[type_led].pin, 0);
}