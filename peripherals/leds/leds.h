/*boton chiquito - GP16
boton 1 - GP18
boton 2 - GP19
boton 3 - GP20
boton 4 - GP21
azul - GP22
verde - GP26
amarillo - GP27
rojo - GP28
*/

#include <stdio.h>


enum type_led {
    RED_LED = 0,
    BLUE_LED,
    YELLOW_LED,
    GREEN_LED,
};

void init_leds();
void on_led(type_led);
void off_led(type_led);