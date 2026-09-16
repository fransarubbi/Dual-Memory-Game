#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "keyboard.h"
#include "buffer/buffer.h"

typedef struct {
    uint8_t scan_row;
    uint8_t scan_col;
    uint8_t key_row;
    uint8_t key_col;
    uint8_t key_code;      
    bool key_ok;
} Scan;

static Scan scan;
static StateKey state_key;     

// GP7 - R0
// GP6 - R1
// GP5 - R2
// GP4 - R3
// GP3 - C0
// GP2 - C1
// GP1 - C2
// GP0 - C3

static uint8_t pins_rows[4] = {7, 6, 5, 4};
static uint8_t pins_cols[4] = {3, 2, 1, 0};

static void init_scan(void) {
    scan.scan_row = 0;
    scan.scan_col = 0;
    scan.key_row = 0;
    scan.key_col = 0;
    scan.key_code = 0;
    scan.key_ok = false;
}

static Event decoder(uint8_t code) {
    static const char map[16] = {
        ONE,    TWO,    THREE,  LETTER_A,
        FOUR,   FIVE,   SIX,    LETTER_B,
        SEVEN,  EIGHT,  NINE,   LETTER_C,
        STAR,   ZERO,   NUMERAL,LETTER_D
    };
    return map[code];
}

void init_keyboard(void) {
    for(uint8_t i = 0; i < 4; i++){
        // configurar las filas como salidas
        gpio_init(pins_rows[i]);
        gpio_set_dir(pins_rows[i], GPIO_OUT);
        // inicializar en 1 para la logica Pull-Up
        gpio_put(pins_rows[i], 1); 
    }

    for(uint8_t i = 0; i < 4; i++){
        // configurar las columnas como entradas, con resistencias de pull-up
        gpio_init(pins_cols[i]);            
        gpio_set_dir(pins_cols[i], GPIO_IN);
        gpio_pull_up(pins_cols[i]);
    }

    init_scan();
    state_key = WAIT_KEYBOARD;
    sleep_ms(DELAY_SCAN);
}

static uint8_t read_keyboard(uint8_t row){
    // poner todas las filas en 1
    for(uint8_t i = 0; i < 4; i++){
        gpio_put(pins_rows[i], 1);
    }

    // poner solo la fila a escanear en 0
    gpio_put(pins_rows[row], 0);
    
    // retardo minimo para que los cables se descarguen y el voltaje se estabilice
    sleep_us(2);

    // leer columnas buscando un 0 debido al Pull-Up
    if (gpio_get(pins_cols[0]) == 0) return 0;   
    else if (gpio_get(pins_cols[1]) == 0) return 1;
    else if (gpio_get(pins_cols[2]) == 0) return 2;
    else if (gpio_get(pins_cols[3]) == 0) return 3;
    else return 4; // ninguna tecla pulsada en esta fila
}

void update_fsm_keyboard(void){
    switch (state_key){
        case WAIT_KEYBOARD:
            sleep_ms(DELAY_SCAN);
            scan.scan_row = (scan.scan_row + 1) & 3; 
            scan.scan_col = read_keyboard(scan.scan_row);

            if(scan.scan_col != 4){ 
                scan.key_row = scan.scan_row;
                scan.key_col = scan.scan_col;
                state_key = DEBOUNCE_KEYBOARD;
            }
            break;

        case DEBOUNCE_KEYBOARD:
            sleep_ms(DELAY_DEBOUNCE);
            scan.scan_col = read_keyboard(scan.key_row); // escanear la fila guardada
            
            if(scan.scan_col == scan.key_col){
                state_key = PRESS_KEYBOARD;
            } else{
                state_key = WAIT_KEYBOARD;
            }
            break;

        case PRESS_KEYBOARD:
            scan.scan_col = read_keyboard(scan.key_row);
            
            // si devuelve 4, ya solto el boton
            if(scan.scan_col == 4){ 
                scan.key_ok = true;
                scan.key_code = scan.key_row * COLS + scan.key_col;
                Event event = decoder(scan.key_code);
                insert_buffer(&buffer_key, event, INSERTED);
                state_key = WAIT_KEYBOARD;
            }
            break;
    }
}