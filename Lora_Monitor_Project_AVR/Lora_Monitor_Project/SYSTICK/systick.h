/*************************************************************************
* Description: Libreria para la configuracion del contador general del sistema
* Author:      Ing. Misael Salazar
* File:        systick.h
* Target:      ATMEGA328P
**************************************************************************/

#ifndef SYSTICK_H_
#define SYSTICK_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#define CARGATIMER 0x7D0          // para 1ms

#define PREESCALER_1      1
#define PREESCALER_8      8
#define PREESCALER_64     64
#define PREESCALER_256    256
#define PREESCALER_1024   1024

extern volatile unsigned long systick_count;

/**
 @brief Inicia el systick
 @param  none
 @return none
 */
void init_systick();

/**
 @brief Apaga el systick
 @param  none
 @return none
 */
void systick_off();

#endif /* SYSTICK_H_ */