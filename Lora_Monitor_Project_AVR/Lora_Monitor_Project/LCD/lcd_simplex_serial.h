/*************************************************************************
* Description: Libreria para el control del lcd 16x2 y 20x4
* Author:      Ing. Misael Salazar
* File:        lcd_simplex_serial.h
* Target:      ATMEGA
**************************************************************************/

#ifndef LCD_SIMPLEX_SERIAL_H_
#define LCD_SIMPLEX_SERIAL_H_

#include <avr/io.h>
#include "../config_cpu.h"
#include "i2cmaster.h"

#define LCD_BACKLIGHT         0x08
#define LCD_NOBACKLIGHT       0x00
#define LCD_FIRST_ROW         0x80
#define LCD_SECOND_ROW        0xC0
#define LCD_THIRD_ROW         0x94
#define LCD_FOURTH_ROW        0xD4
#define LCD_CLEAR             0x01
#define LCD_RETURN_HOME       0x02
#define LCD_ENTRY_MODE_SET    0x04
#define LCD_CURSOR_OFF        0x0C
#define LCD_UNDERLINE_ON      0x0E
#define LCD_BLINK_CURSOR_ON   0x0F
#define LCD_MOVE_CURSOR_LEFT  0x10
#define LCD_MOVE_CURSOR_RIGHT 0x14
#define LCD_TURN_ON           0x0C
#define LCD_TURN_OFF          0x08
#define LCD_SHIFT_LEFT        0x18
#define LCD_SHIFT_RIGHT       0x1E
#define LCD_TYPE              2 // 0 -> 5x7 | 1 -> 5x10 | 2 -> 2 lines
#define PORTLCD               PORTD
#define DDRLCD                DDRD
#define LCD_I2C_ADDRESS       0x4E

/**
 @brief Inicializa el lcd
 @return void
 */
void lcd_init();

/**
 @brief Establece la posicion del cursor
 @param row
 @param col
 @return void
 */
void lcd_set_cursor(const unsigned char row, const unsigned char col);

/**
 @brief Recibe un string y lo separa en caracteres
 @param char*
 @return void
 */
void lcd_write_string(const char* str);

/**
 @brief Borra el contenido de la pantalla
 @return void
 */
void lcd_clear();

#endif /* LCD_SIMPLEX_SERIAL_H_ */