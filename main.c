#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "engine/led/led_engine.h"
#include "engine/word/word_engine.h"


int main(void) {
    const uint LED_PIN = 28;

    multicore_launch_core1(led_game);
    multicore_launch_core1(word_game);

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    while (true) {
        gpio_put(LED_PIN, 1);
        sleep_ms(500);

        gpio_put(LED_PIN, 0);
        sleep_ms(500);
    }
    return 0;
}

// cmake --build build    asi se compila