#include <stdint.h>

typedef enum {
    RED_LED,
    BLUE_LED,
    GREEN_LED,
    YELLOW_LED,
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

void insert(Event event, Action action[], Operation op) {
    if (op == ORIGINAL) {

    } else {
        
    }
}