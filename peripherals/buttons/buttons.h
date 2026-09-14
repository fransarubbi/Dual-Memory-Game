#include <stdio.h>

enum type_button {
    BUTTON_OK,
    BUTTON_RED,
    BUTTON_BLUE,
    BUTTON_YELLOW,
    BUTTON_GREEN
};

void init_buttons();
void read_button(type_button);
