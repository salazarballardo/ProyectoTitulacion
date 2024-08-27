/*************************************************************************
* Description: Libreria para lectura de temperatura y presion con el sensor DHT11
*              de forma asincrona (sin el delay de 18ms)
* Author:      Ing. Misael Salazar
* File:        dht11_async.h
* Target:      ATMEGA328P
**************************************************************************/

#ifndef DHT11_ASYNC_H_
#define DHT11_ASYNC_H_

#include <avr/io.h>
#include "../config_cpu.h"
#include <util/delay.h>
#include "../bit_io.h"

#define DDRPORTDHT DDRB
#define PORTDATADHT PORTB
#define PORDATAREAD PINB
#define PINDATADHT 0
#define DELAYDATADHT 85

/**
 @brief Envia el pulso de inicio al dht11
 @param  none
 @return none
 */
void startpulse_dht11();

/**
 @brief Lanza la lectura de presion y temperatura del dht11
 @param  float* temperature : retorna la temperatura por referencia
 @param float* humidity : retorna la humedad por referencia
 @return unsigned char : retorna un 1 en caso de exito de lectura o cero si hay error
 */
unsigned char read_dht11(float* temperature, float* humidity);

#endif /* DHT11_ASYNC_H_ */




















































