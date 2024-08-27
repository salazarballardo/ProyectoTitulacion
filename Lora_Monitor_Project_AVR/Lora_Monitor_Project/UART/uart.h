/*************************************************************************
* Description: Libreria para la configuracion y manejo del UART en ATMEGA	
* Author:      Ing. Misael Salazar
* File:        uart.h
* Target:      ATMEGA328P
**************************************************************************/

#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include "../config_cpu.h"
#include <avr/interrupt.h>

#define RX_BUFFER_SIZE   120      // tamanio del buffer de recepcion

#define BAUD_RATE_1200   0
#define BAUD_RATE_2400   0
#define BAUD_RATE_4800   0
#define BAUD_RATE_9600   1
#define BAUD_RATE_19200  0
#define BAUD_RATE_28800  0
#define BAUD_RATE_38400  0
#define BAUD_RATE_57600  0
#define BAUD_RATE_76800  0
#define BAUD_RATE_115200 0

// F_CPU 16MHZ
//#define BAUD_RATE_1200_UBRR	  832
//#define BAUD_RATE_2400_UBRR     415
//#define BAUD_RATE_4800_UBRR	  207
//#define BAUD_RATE_9600_UBRR     103  
//#define BAUD_RATE_19200_UBRR    51
//#define BAUD_RATE_28800_UBRR    33
//#define BAUD_RATE_38400_UBRR    25
//#define BAUD_RATE_57600_UBRR    16
//#define BAUD_RATE_76800_UBRR    12
//#define BAUD_RATE_115200_UBRR   16

#define BAUD_RATE_1200_BPS	   1200
#define BAUD_RATE_2400_BPS     2400
#define BAUD_RATE_4800_BPS	   4800
#define BAUD_RATE_9600_BPS     9600
#define BAUD_RATE_19200_BPS    19200
#define BAUD_RATE_28800_BPS    28800
#define BAUD_RATE_38400_BPS    38400
#define BAUD_RATE_57600_BPS    57600
#define BAUD_RATE_76800_BPS    76800
#define BAUD_RATE_115200_BPS   115200

#define CALC_UBRR_16(BAUD_RATE) (F_CPU/16/BAUD_RATE) -1
#define CALC_UBRR_8(BAUD_RATE) (F_CPU/8/BAUD_RATE) -1

// variables globales
extern char rx_buffer[RX_BUFFER_SIZE];
extern volatile unsigned char uartmsg;             // flag de nuevo mensaje entrante por RX

/**
 @brief Inicializa el UART
 @param  none
 @return none
 */
void uart_init();

/**
 @brief Habilita la interrupcion por caracter recibiido a traves del UART
 @param  none
 @return none
 */
void enable_int_uart();

/**
 @brief Deshabilita la interrupcion por caracter recibiido a traves del UART
 @param  none
 @return none
 */
void disable_int_uart();

/**
 @brief Funcion con el codigo que se ejecutara en la interrupcion por RX
 @param  none
 @return none
 */
void handler_rxinterrupt();

/**
 @brief Envia un mensaje a traves del UART
 @param  const char* s
 @return none
 */
void uart_write(const char* s);

#endif /* UART_H_ */





























































