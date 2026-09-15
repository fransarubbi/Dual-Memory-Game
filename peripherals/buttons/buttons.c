#include <stdio.h>
#include "pico/stdlib.h"
#include "peripherals/buttons/buttons.h"
#include "buffer/buffer.h"
#include "peripherals/leds/leds.h"

/*
BUTTON_OK 16,
BUTTON_BLUE 18,
BUTTON_GREEN 19
BUTTON_YELLOW 20,
BUTTON_RED 21,
*/

typedef enum {
    WAIT_BUTTON, 
    DEBOUNCE_BUTTON, 
    PRESS_BUTTON
} StateButton;

typedef struct {
    unsigned int pin;
    StateButton state;
    uint32_t last_debounce;
} Button;

static Button buttons[5] = {
    {16, WAIT_BUTTON, 0},
    {18, WAIT_BUTTON, 0},
    {19, WAIT_BUTTON, 0},
    {20, WAIT_BUTTON, 0},
    {21, WAIT_BUTTON, 0},
};

void init_buttons() {
    for (uint8_t i = 0; i < 5; i++) {
        gpio_init(buttons[i].pin);
        gpio_set_dir(buttons[i].pin, GPIO_IN);
        gpio_pull_up(buttons[i].pin);
    }
}

static Event convert_button_to_event(unsigned int pin) {
    if (pin == 16) {
        Event event = OK;
        return event;
    } else if (pin == 18) {
        Event event = BLUE_LED;
        return event;
    } else if (pin == 19) {
        Event event = GREEN_LED;
        return event;
    } else if (pin == 20) {
        Event event = YELLOW_LED;
        return event;
    } else if (pin == 21) {
        Event event = RED_LED;
        return event;
    }

} 

static type_led convert_button_to_led(unsigned int pin) {
    if (pin == 18) {
        type_led led = TYPE_BLUE_LED;
        return led;
    } else if (pin == 19) {
        type_led led = TYPE_GREEN_LED;
        return led;
    } else if (pin == 20) {
        type_led led = TYPE_YELLOW_LED;
        return led;
    } else if (pin == 21) {
        type_led led = TYPE_RED_LED;
        return led;
    }
} 

static void fsm(Button *button) {
    if (button->pin == 16) {
        switch (button->state) {
            case WAIT_BUTTON:
                if (!gpio_get(button->pin)) {
                    printf("BUTTON: OK pulsado\n");
                    button->last_debounce = to_ms_since_boot(get_absolute_time());
                    button->state = DEBOUNCE_BUTTON;
                }
            break;

            case DEBOUNCE_BUTTON:
                if ((to_ms_since_boot(get_absolute_time()) - button->last_debounce) >= DEBOUNCE_TIME) {
                    if (!gpio_get(button->pin)) {
                        printf("BUTTON: OK sigue pulsado\n");
                        button->state = PRESS_BUTTON;
                        Event event = OK;
                        insert_buffer(&buffer_leds, event, INSERTED);
                    } else {
                        button->state = WAIT_BUTTON;
                    }
                }
            break;

            case PRESS_BUTTON:
                if (gpio_get(button->pin)) {
                    printf("BUTTON: OK liberado\n");
                    button->state = WAIT_BUTTON;
                }
            break;
        }
    } else {
        switch (button->state) {
            case WAIT_BUTTON:
                if (!gpio_get(button->pin)) {
                    printf("BUTTON: %u pulsado\n", button->pin);
                    button->last_debounce = to_ms_since_boot(get_absolute_time());
                    button->state = DEBOUNCE_BUTTON;
                }
            break;

            case DEBOUNCE_BUTTON:
                if ((to_ms_since_boot(get_absolute_time()) - button->last_debounce) >= DEBOUNCE_TIME) {
                    if (!gpio_get(button->pin)) {
                        printf("BUTTON: %u sigue pulsado\n", button->pin);
                        button->state = PRESS_BUTTON;
                        Event event = convert_button_to_event(button->pin);
                        insert_buffer(&buffer_leds, event, INSERTED);
                        type_led led = convert_button_to_led(button->pin);
                        on_led(led);
                    } else {
                        button->state = WAIT_BUTTON;
                    }
                }
            break;

            case PRESS_BUTTON:
                if (gpio_get(button->pin)) {
                    printf("BUTTON: %u liberado\n", button->pin);
                    type_led led = convert_button_to_led(button->pin);
                    off_led(led);
                    button->state = WAIT_BUTTON;
                }
            break;
        }
    }
}

void read_buttons() {
    for (uint8_t i = 0; i < 5; i++) {
        fsm(&buttons[i]);
    }
}
