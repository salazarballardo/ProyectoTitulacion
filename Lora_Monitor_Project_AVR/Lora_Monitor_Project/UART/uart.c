/*************************************************************************
* Description: Libreria para la configuracion y manejo del UART en ATMEGA	
* Author:      Ing. Misael Salazar
* File:        uart.c
* Target:      ATMEGA328P
**************************************************************************/

#include "uart.h"

// funciones estaticas
static void UART_putc(unsigned char data);
static volatile unsigned char rx_read_pos;

ISR(USART_RX_vect)       //Interrupcion mensaje entrante en RX
{
	handler_rxinterrupt();
}

/*************************************************************************
Inicializa el UART
* Datos de 8 bits
* USART como UART
* Paridad desactivada
* Stops = 1
* Datos de 8 bits
param void
return void
*************************************************************************/
void uart_init()
{
	// Inicializamos variables
	rx_read_pos = 0;
	uartmsg = 0;
	
	// Datos de 8 bits
	UCSR0C |=  (1<<UCSZ00);
	UCSR0C |=  (1<<UCSZ01);
	
	#if BAUD_RATE_1200 == 1
		UBRR0 = CALC_UBRR_16(BAUD_RATE_1200_BPS);
	#elif BAUD_RATE_2400 == 1		
		UBRR0 = CALC_UBRR_16(BAUD_RATE_2400_BPS);
	#elif BAUD_RATE_4800 == 1
		UBRR0 = CALC_UBRR_16(BAUD_RATE_4800_BPS);
	#elif BAUD_RATE_9600 == 1
		UBRR0 = CALC_UBRR_16(BAUD_RATE_9600_BPS);
	#elif BAUD_RATE_19200 == 1 	
		UBRR0 = CALC_UBRR_16(BAUD_RATE_19200_BPS);
	#elif BAUD_RATE_28800 == 1
		UBRR0 = CALC_UBRR_16(BAUD_RATE_28800_BPS);
	#elif BAUD_RATE_38400 == 1
		UBRR0 = CALC_UBRR_16(BAUD_RATE_38400_BPS);
	#elif BAUD_RATE_57600 == 1
		UBRR0 = CALC_UBRR_16(BAUD_RATE_57600_BPS);
	#elif BAUD_RATE_76800 == 1
		UBRR0 = CALC_UBRR_16(BAUD_RATE_76800_BPS);
	#elif BAUD_RATE_115200 == 1
		UCSR0A |= (1<<U2X0);
		UBRR0 = CALC_UBRR_8(BAUD_RATE_115200_BPS);
	#endif
	
	// enable the transmitter and receiver
	UCSR0B |=  (1 << TXEN0);
	UCSR0B |=  (1 << RXEN0);
}

/*************************************************************************
Habilita la interrupcion por caracter recibido a traves del UART
param void
return void
*************************************************************************/
void enable_int_uart()
{
	//habilitamos la interrupcion por RX
	UCSR0B |= (1<<RXCIE0);
}

/*************************************************************************
Deshabilita la interrupcion por caracter recibiido a traves del UART
param void
return void
*************************************************************************/
void disable_int_uart()
{
	//Deshabilitamos la interrupcion por RX
	UCSR0B &= ~(1<<RXCIE0);
}

/*************************************************************************
Funcion con el codigo que se ejecutara en la interrupcion por RX
param void
return void
*************************************************************************/
void handler_rxinterrupt()
{
	rx_buffer[rx_read_pos] = UDR0;
	
	if (rx_buffer[rx_read_pos] != '\r')                  //si no es el fin de la cadena
	{
		rx_read_pos++;
	}
	else
	{
		rx_buffer[rx_read_pos] = 0;                     // Nos aseguramos que el buffer termina en cero
		uartmsg = 1;		
		rx_read_pos = 0;
	}
}

/*************************************************************************
Envia un caracter a traves del UART
param unsigned char character
return void
*************************************************************************/
static void UART_putc(unsigned char character)
{
	// wait for transmit buffer to be empty
	while(!(UCSR0A & (1 << UDRE0)));
	// load data into transmit register
	UDR0 = character;
}

/*************************************************************************
Envia un mensaje a traves del UART
param const char* s
return void
*************************************************************************/
void uart_write(const char* s)         
{
	// transmit character until NULL is reached
	while(*s != '\0') 
		UART_putc(*s++);
}