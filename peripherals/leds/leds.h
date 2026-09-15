#ifndef LEDS_H_
#define LEDS_H_

#include <stdio.h>


typedef enum {
    TYPE_RED_LED,
    TYPE_BLUE_LED,
    TYPE_GREEN_LED,
    TYPE_YELLOW_LED
} type_led;

void init_leds();
void on_led(type_led);
void off_led(type_led);

#endif