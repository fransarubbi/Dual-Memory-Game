#ifndef BUTTONS_H_
#define BUTTONS_H_

#include <stdio.h>
#define DEBOUNCE_TIME 80

typedef enum {
    BUTTON_OK,
    BUTTON_RED,
    BUTTON_BLUE,
    BUTTON_YELLOW,
    BUTTON_GREEN
} type_button;

void init_buttons();
void read_buttons();

#endif