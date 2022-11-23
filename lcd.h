#pragma once
#include "hardware/i2c.h"
#include <stdbool.h>

#define LCD_CLEAR 0x01
#define LCD_CURSOR_RIGHT 0x06
#define LCD_CURSOR_PRIMARY_LINE 0x80
#define LCD_4_BIT 0x28

typedef struct lcd_instance {
	uint8_t address;
	i2c_inst_t* i2c;
} lcd_instance_t;

void lcd_init(lcd_instance_t* lcd, bool cursor, bool blink);
void lcd_cursorpos(lcd_instance_t* lcd, uint8_t pos);
void lcd_clear(lcd_instance_t* lcd);

void lcd_putc(lcd_instance_t* lcd, char c);
void lcd_puts(lcd_instance_t* lcd, const char* s);

uint8_t i2c_find_first_device(i2c_inst_t* i2c);