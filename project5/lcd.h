/**
 * lcd.h
 * Copyright (C) 2001-2020, Tony Givargis
 */

#ifndef _LCD_H_
#define _LCD_H_

void lcd_init();

void lcd_clr(void);

void lcd_pos(unsigned char r, unsigned char c);

void lcd_put(char c);

void lcd_puts(const char *s);

void lcd_print(const char *s, unsigned char r, unsigned char c);

#endif /* _LCD_H_ */
