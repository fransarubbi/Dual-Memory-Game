#include <stdio.h>
#include <stdint.h>
#include "buffer/buffer.h"

Buffer buffer_leds; 
Buffer buffer_key; 

void clean_buffer(Buffer *buffer) {
    buffer->idx_inserted = 0;
    buffer->idx_original = 0;
}

bool insert_buffer(Buffer *buffer, Event event, Operation op) {
    if (op == ORIGINAL) {
        if (buffer->idx_original == MAX_BUFFER) {
            return false;
        }
        Action action = buffer->action[buffer->idx_original];
        action.original = event;
        buffer->action[buffer->idx_original] = action;
        buffer->idx_original++;
        return true;
    } else if (op == INSERTED) {
        if (buffer->idx_inserted == MAX_BUFFER) {
            return false;
        }
        Action action = buffer->action[buffer->idx_inserted];
        action.inserted = event;
        buffer->action[buffer->idx_inserted] = action;
        buffer->idx_inserted++;
        return true;
    } else {
        return false;
    }
}

bool compare_buffer(Buffer *buffer) {
    if ((buffer->idx_original + 1) != buffer->idx_inserted) {
        printf("BUFFER: idx_original: %d, idx_inserted: %d\n", buffer->idx_original, buffer->idx_inserted);
        return false;
    }
    for (uint8_t i = 0; i < buffer->idx_original; i++) {
        Action action = buffer->action[i];
        printf("BUFFER: original: %d , inserted: %d\n", action.original, action.inserted);
        if (action.original != action.inserted) {
            return false;
        }
    }
    Action action = buffer->action[buffer->idx_inserted - 1];
    if (action.inserted == OK) {
        printf("BUFFER: OK encontrado\n");
        return true;
    }
    return false;
}

Event get_event(Buffer *buffer, Operation op, uint8_t idx) {
    if (op == ORIGINAL) {
        Action action = buffer->action[idx];
        return action.original;
    } else if (op == INSERTED) {
        Action action = buffer->action[idx];
        return action.inserted;
    }
}

bool is_ok(Buffer *buffer) {
    if (buffer->idx_inserted == 0) {
        return false;
    }
    Action action = buffer->action[buffer->idx_inserted - 1];
    if (action.inserted == OK) {
        return true;
    }
    return false;
}

void clean_inserted(Buffer *buffer) {
    buffer->idx_inserted = 0;
}