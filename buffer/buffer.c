#include <stdint.h>
#include "buffer/buffer.h"


void clean_buffer(Buffer *buffer) {
    buffer->idx_inserted = 0;
    buffer->idx_original = 0;
}

bool insert_buffer(Buffer *buffer, Event event, Operation op) {
    if (op == ORIGINAL) {
        if (buffer->idx_original == MAX) {
            return false;
        }
        Action action = buffer->action[buffer->idx_original];
        action.original = event;
        buffer->action[buffer->idx_original] = action;
        buffer->idx_original++;
        return true;
    } else if (op == INSERTED) {
        if (buffer->idx_inserted == MAX) {
            return false;
        }
        Action action = buffer->action[buffer->idx_inserted];
        action.original = event;
        buffer->action[buffer->idx_inserted] = action;
        buffer->idx_inserted++;
        return true;
    } else {
        return false;
    }
}

bool compare_buffer(Buffer *buffer) {
    if ((buffer->idx_original + 1) != buffer->idx_inserted) {
        return false;
    }
    for (uint8_t i = 0; i < buffer->idx_original; i++) {
        Action action = buffer->action[i];
        if (action.original != action.inserted) {
            return false;
        }
    }
    Action action = buffer->action[buffer->idx_inserted - 1];
    if (action.inserted == OK) {
        return true;
    }
    return false;
}