#include <stdio.h>


enum type_led {
    RED_LED = 0,
    BLUE_LED,
    YELLOW_LED,
    GREEN_LED,
};

void init_leds();
void on_led(type_led);
void off_led(type_led);