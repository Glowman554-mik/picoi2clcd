#include "lcd.h"
#include "hardware/i2c.h"
#include "pico/time.h"
#include <stdio.h>

void lcd_wait() {
	sleep_us(1000);
}

void lcd_send_byte(lcd_instance_t* lcd, char b, uint8_t rw, uint8_t rs) {
	char data[1];
	data[0] = (b & 0xF0) + 0x08 + ((rw & 0x01) << 1) + (rs & 0x01);
	i2c_write_blocking (lcd->i2c, lcd->address, data, 1, false);
	lcd_wait();
	data[0] = (b & 0xF0) + 0xC + ((rw & 0x01) << 1) + (rs & 0x01);
	i2c_write_blocking (lcd->i2c, lcd->address, data, 1, false);
	lcd_wait();
	data[0] = (b & 0xF0) + 0x8 + ((rw & 0x01) << 1) + (rs & 0x01);
	i2c_write_blocking (lcd->i2c, lcd->address, data, 1, false);
	lcd_wait();
	data[0] = ((b & 0xF) << 4) + 0x8 + ((rw & 0x01) << 1) + (rs & 0x01);
	i2c_write_blocking (lcd->i2c, lcd->address, data, 1, false);
	lcd_wait();
	data[0] = ((b & 0xF) << 4) + 0xC + ((rw & 0x01) << 1) + (rs & 0x01);
	i2c_write_blocking (lcd->i2c, lcd->address, data, 1, false);
	lcd_wait();
	data[0] = ((b & 0xF) << 4) + 0x8 + ((rw & 0x01) << 1) + (rs & 0x01);
	i2c_write_blocking (lcd->i2c, lcd->address, data, 1, false);
	lcd_wait();
}
void lcd_send_nippel(lcd_instance_t* lcd, char b, uint8_t rw, uint8_t rs) {
	char data[1];
	data[0] = ((b & 0xF) << 4) + 0x0 + ((rw & 0x01) << 1) + (rs & 0x01);
	i2c_write_blocking (lcd->i2c, lcd->address, data, 1, false);
	lcd_wait();
	data[0] = ((b & 0xF) << 4) + 0x4 + ((rw & 0x01) << 1) + (rs & 0x01);
	i2c_write_blocking (lcd->i2c, lcd->address, data, 1, false);
	lcd_wait();
	data[0] = ((b & 0xF) << 4) + 0x0 + ((rw & 0x01) << 1) + (rs & 0x01);
	i2c_write_blocking (lcd->i2c, lcd->address, data, 1, false);
	lcd_wait();
}

uint8_t lcd_display_ctrl_builder(bool display, bool cursor, bool blink) {
	return 0x08 | (display ? 0x04 : 0x00) | (cursor ? 0x02 : 0x00) | (blink ? 0x01 : 0x00);
}

void lcd_init(lcd_instance_t* lcd, bool cursor, bool blink) {
	sleep_us(20000);
	lcd_send_nippel(lcd, 0b0011, 0, 0);
	sleep_us(5000);
	lcd_send_nippel(lcd, 0b0011, 0, 0);
	sleep_us(1000);
	lcd_send_nippel(lcd, 0b0011, 0, 0);

	lcd_send_nippel(lcd, 0b0010, 0, 0);

	lcd_send_byte(lcd, LCD_4_BIT, 0, 0);
	lcd_send_byte(lcd, LCD_CLEAR, 0, 0);
	lcd_send_byte(lcd, LCD_CURSOR_RIGHT, 0, 0);
	lcd_send_byte(lcd, LCD_CURSOR_PRIMARY_LINE, 0, 0);
	lcd_send_byte(lcd, lcd_display_ctrl_builder(true, cursor, blink), 0, 0);

	lcd_clear(lcd);
}

void lcd_cursorpos(lcd_instance_t* lcd, uint8_t pos) {
	lcd_send_byte(lcd, LCD_CURSOR_PRIMARY_LINE + pos, 0, 0);
}

void lcd_clear(lcd_instance_t* lcd) {
	lcd_send_byte(lcd, LCD_CLEAR, 0, 0);
	lcd_cursorpos(lcd, 0x0);
}

void lcd_putc(lcd_instance_t* lcd, char c) {
	lcd_send_byte(lcd, c, 0, 1);
}

void lcd_puts(lcd_instance_t* lcd, const char* s) {
	while (*s) {
		lcd_putc(lcd, *s++);
	}
}

uint8_t i2c_find_first_device(i2c_inst_t* i2c) {
	for (uint8_t address = 0; address < 255; address++) {
		const char sdata[] = {0x55};
		char data[1];
		i2c_write_blocking (i2c, address, sdata, 1, false);
		i2c_read_blocking(i2c, address, data, 1, false);
		if (data[0] == 0x55) {
			return address;
		}
	}

	printf("Could not find device!");
	while(true);
}