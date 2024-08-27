/*************************************************************************
* Description: Libreria para envio de mensajes a traves del puerto serie 
* por medio del pin seleccionado. 
* Author:      Ing. Misael Salazar
* File:        send_serial.h
* Target:      ATMEGA328P
**************************************************************************/

#ifndef SEND_SERIAL_H_
#define SEND_SERIAL_H_

#include <avr/io.h>
#include "../config_cpu.h"

#define SERIAL_RATE_1200   0
#define SERIAL_RATE_2400   0
#define SERIAL_RATE_4800   0
#define SERIAL_RATE_9600   1
#define SERIAL_RATE_19200  0
#define SERIAL_RATE_28800  0
#define SERIAL_RATE_38400  0
#define SERIAL_RATE_57600  0
#define SERIAL_RATE_76800  0
#define SERIAL_RATE_115200 0

#define SERIAL_BAUD_RATE_1200_BPS	  1200
#define SERIAL_BAUD_RATE_2400_BPS     2400
#define SERIAL_BAUD_RATE_4800_BPS	  4800
#define SERIAL_BAUD_RATE_9600_BPS     9600
#define SERIAL_BAUD_RATE_19200_BPS    19200
#define SERIAL_BAUD_RATE_28800_BPS    28800
#define SERIAL_BAUD_RATE_38400_BPS    38400
#define SERIAL_BAUD_RATE_57600_BPS    57600
#define SERIAL_BAUD_RATE_76800_BPS    76800
#define SERIAL_BAUD_RATE_115200_BPS   115200

#define SERIAL_CALC_UBRR_16(BAUD_RATE) (F_CPU/16/BAUD_RATE)
#define SERIAL_CALC_UBRR_8(BAUD_RATE) (F_CPU/8/BAUD_RATE)

/**
 @brief Envia un mensaje serial a traves del pin seleccionado
 @param char* str
 @return void
 */
void send_serial_write(uint8_t* DDR_SERIAL, uint8_t* PORT_SERIAL, uint8_t PIN_SERIAL, const char* str);

#endif /* SEND_SERIAL_H_ */