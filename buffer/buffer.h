#ifndef BUFFER_H_
#define BUFFER_H_

#include <stdint.h>
#include <stdbool.h>

#define MAX_BUFFER 32

typedef enum {
    // Leds
    RED_LED,
    BLUE_LED,
    GREEN_LED,
    YELLOW_LED,

    // Ambos
    OK,

    // Teclado
    ZERO,
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    LETTER_A,
    LETTER_B,
    LETTER_C,
    LETTER_D,
} Event;

typedef struct {
    Event original;
    Event inserted;
} Action;

typedef enum {
    ORIGINAL,
    INSERTED,
} Operation;

typedef struct {
    Action action[MAX_BUFFER];
    uint8_t idx_original;
    uint8_t idx_inserted;
} Buffer;

extern Buffer buffer_leds; 
extern Buffer buffer_key; 

void clean_buffer(Buffer*);
bool insert_buffer(Buffer*, Event, Operation op);
bool compare_buffer(Buffer*);
Event get_event(Buffer*, Operation, uint8_t);
bool is_ok(Buffer*);
void clean_inserted(Buffer*);

#endif