#include "pico/stdlib.h"
#include "peripherals/display/display.h"
#include "hardware/i2c.h"



// envia un byte en bruto al expansor PCF8574
static void i2c_write_byte(uint8_t val) {
    i2c_write_blocking(I2C_PORT, LCD_ADDR, &val, 1, false);
}

// envia un pulso en el pin Enable (E) para que el LCD procese el dato
static void lcd_toggle_enable(uint8_t val) {
    sleep_us(600);
    i2c_write_byte(val | LCD_ENABLE);
    sleep_us(600);
    i2c_write_byte(val & ~LCD_ENABLE);
    sleep_us(600);
}

// envia un nibble (4 bits) al display
static void lcd_send_nibble(uint8_t nibble, uint8_t mode) {
    uint8_t data = (nibble & 0xF0) | mode | LCD_BACKLIGHT;
    i2c_write_byte(data);
    lcd_toggle_enable(data);
}

// envia un byte completo dividido en dos paquetes de 4 bits
void lcd_send_byte(uint8_t val, uint8_t mode) {
    lcd_send_nibble(val & 0xF0, mode);        // Parte alta
    lcd_send_nibble((val << 4) & 0xF0, mode); // Parte baja
}

// Modo: 0 = Comando, 1 = Dato (caracter)
void lcd_send_cmd(uint8_t cmd) { lcd_send_byte(cmd, 0); }
void lcd_send_char(char c)     { lcd_send_byte((uint8_t)c, 1); }

void lcd_init(uint sda_pin, uint scl_pin) {
    // inicializar bus I2C a 100 kHz
    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(sda_pin, GPIO_FUNC_I2C);
    gpio_set_function(scl_pin, GPIO_FUNC_I2C);
    gpio_pull_up(sda_pin);
    gpio_pull_up(scl_pin);

    sleep_ms(50); // esperar estabilizacion de encendido

    lcd_send_nibble(0x30, 0); sleep_ms(5);
    lcd_send_nibble(0x30, 0); sleep_us(150);
    lcd_send_nibble(0x30, 0); sleep_us(150);
    lcd_send_nibble(0x20, 0); sleep_ms(2); // Pasar a 4 bits

    lcd_send_cmd(0x28); // 2 líneas, fuente 5x8
    lcd_send_cmd(0x0C); // Encender display, apagar cursor
    lcd_send_cmd(0x06); // Modo auto-incremento de cursor
    lcd_send_cmd(0x01); // Limpiar pantalla
    sleep_ms(2);
}

void lcd_set_cursor(int row, int col) {
    int row_offsets[] = {0x00, 0x40};
    lcd_send_cmd(0x80 | (col + row_offsets[row]));
}

void lcd_print(const char *str) {
    while (*str) {
        lcd_send_char(*str++);
    }
}