#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <stdint.h>

#define	DELAY_DEBOUNCE 80
#define DELAY_SCAN 10
#define ROWS 4
#define COLS 4

typedef enum{
	WAIT_KEYBOARD,
	DEBOUNCE_KEYBOARD,
	PRESS_KEYBOARD
} StateKey;

void init_keyboard(void);
void fsm_keyboard(void);


#endif