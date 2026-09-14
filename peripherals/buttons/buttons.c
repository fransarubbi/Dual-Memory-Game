#include "pico/stdlib.h"
#include "peripherals/buttons/buttons.h"

/*
BUTTON_OK 16,
BUTTON_BLUE 18,
BUTTON_GREEN 19
BUTTON_YELLOW 20,
BUTTON_RED 21,
*/

typedef struct {
    unsigned int pin;
    gpio_dir mode;
} Buttons;


static const Buttons buttons[5] = {
    {16, GPIO_IN},
    {18, GPIO_IN},
    {19, GPIO_IN},
    {20, GPIO_IN},
    {21, GPIO_IN},
};

enum StateButton {WAIT_BUTTON, DEBOUNCE_BUTTON, PRESS_BUTTON};

void init_buttons() {
    for (uint8_t i = 0; i < 5; i++) {
        gpio_init(buttons[i].pin);
    }
}

void read_button(type_button type) {

}