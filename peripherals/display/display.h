#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdbool.h>
#include "buffer/buffer.h"

#define LCD_ADDR 0x27
#define I2C_PORT i2c1
#define LCD_BACKLIGHT 0x08
#define LCD_ENABLE    0x04
#define SDA 14
#define SCL 15

void lcd_init(unsigned int, unsigned int);
void lcd_set_cursor(int, int);
void lcd_print(const char *);
void lcd_send_char(char);
char convert_to_char(Event);

#endif