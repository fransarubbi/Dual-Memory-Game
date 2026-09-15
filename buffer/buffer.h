#include <stdint.h>
#include <stdbool.h>

#define MAX 32

typedef enum {
    RED_LED,
    BLUE_LED,
    GREEN_LED,
    YELLOW_LED,
    OK,
    // aca irian los de tecl matricial
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
    Action action[MAX];
    uint8_t idx_original;
    uint8_t idx_inserted;
} Buffer;

extern Buffer buffer_leds; 
extern Buffer buffer_key; 

void clean_buffer(Buffer*);
bool insert_buffer(Buffer*, Event, Operation op);
bool compare_buffer(Buffer*);