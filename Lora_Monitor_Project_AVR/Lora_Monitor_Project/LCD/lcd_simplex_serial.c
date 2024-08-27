/*************************************************************************
* Description: Libreria para el control del lcd 16x2 y 20x4
* Author:      Ing. Misael Salazar
* File:        lcd_simplex_serial.c
* Target:      ATMEGA
**************************************************************************/

#include "lcd_simplex_serial.h"

static void lcd_write_4Bit(unsigned char nibble);
static void lcd_cmd(const unsigned char cmd);
static void lcd_write_char(const char data);
static void lcd_i2c_write(const unsigned char msj);
unsigned char RS;

/*************************************************************************
Inicializa el lcd, ejecuta la secuencia de inicializacion de la pantalla
param none
return void
*************************************************************************/
void lcd_init()
{
	i2c_init();
    _delay_ms(30);
    lcd_cmd(0x03);
    _delay_ms(5);
    lcd_cmd(0x03);
    _delay_ms(5);
    lcd_cmd(0x03);
    _delay_ms(5);
    lcd_cmd(LCD_RETURN_HOME);
    _delay_ms(5);
    lcd_cmd(0x20 | (LCD_TYPE << 2));
    _delay_ms(50);
    lcd_cmd(LCD_TURN_ON);
    _delay_ms(50);
    lcd_cmd(LCD_CLEAR);
    _delay_ms(50);
    lcd_cmd(LCD_ENTRY_MODE_SET | LCD_RETURN_HOME);
    _delay_ms(50);
}

/*************************************************************************
Envia el nibble mediante el I2C
param unsigned char : msj
return void
*************************************************************************/
static void lcd_i2c_write(unsigned char msj)
{
	i2c_start(LCD_I2C_ADDRESS);
	i2c_write(msj);
	i2c_stop();
}

/*************************************************************************
Envia el nibble hacia el 74hc595
param unsigned char : nibble
return void
*************************************************************************/
static void lcd_write_4Bit(unsigned char nibble)
{
	nibble |= RS;
	lcd_i2c_write(nibble | 0x04);
	_delay_us(80);
	lcd_i2c_write(nibble & 0xFB);
	_delay_us(80);
}

/*************************************************************************
Recibe el comando
unsigned char : cmd
return void
*************************************************************************/
static void lcd_cmd(const unsigned char cmd)
{
	RS = 0; // Command Register Select
	lcd_write_4Bit(cmd & 0xF0);
    lcd_write_4Bit((cmd << 4) & 0xF0);
}

/*************************************************************************
Recibe un char y lo divide en nibble alto y bajo
param char : c
return void
*************************************************************************/
static void lcd_write_char(const char data)
{
	RS = 1;          // data register select
	lcd_write_4Bit(data & 0xF0);
	lcd_write_4Bit((data << 4) & 0xF0);
}

/*************************************************************************
Recibe un string y lo separa en caracteres
param char* : str
return void
*************************************************************************/
void lcd_write_string(const char* str)
{
	for(int i=0; str[i]!='\0'; i++)
	{
		lcd_write_char(str[i]);
	}    
}

/*************************************************************************
Recibe un string y lo separa en caracteres
param unsigned char : row, renglon
param unsigned char : col, columna
return void
*************************************************************************/
void lcd_set_cursor(const unsigned char row, const unsigned char col)
{
	switch(row) 
	{
		case 2:
			lcd_cmd(0xC0 + col-1);
		break;
		case 3:
			lcd_cmd(0x94 + col-1);
		break;
		case 4:
			lcd_cmd(0xD4 + col-1);
		break;
		// Case 1
		default:
			lcd_cmd(0x80 + col-1);
	}
}

/*************************************************************************
Borra el contenido de la pantalla
return void
*************************************************************************/
void lcd_clear()
{
	lcd_cmd(0x01);
    _delay_us(40); 
}














































