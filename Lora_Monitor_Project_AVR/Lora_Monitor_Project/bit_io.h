/*************************************************************************
* Description: Macros para operar con bits
* Author:      Ing. Misael Salazar
* File:        bit_io.h
* Target:      ATMEGA1284P
* Created: 12/09/2022 01:59:34 p. m.
**************************************************************************/
 
#ifndef BIT_IO_H_
#define BIT_IO_H_

#define BIT_SET(SFR,N) (SFR |= (1<<N))
#define BIT_CLEAR(SFR,N) (SFR &= ~(1<<N))
#define TOGGLE_BIT(SFR,N) (SFR ^= (1<<N))
#define BIT_TEST(SFR,N) (SFR & (1<<N))                   // testea el estado de un bit

#define UNTILSET(SFR,N) while (!(SFR & (1<<N)));         // se mantiene en espera hasta que un bit cambia a 1
#define UNTILCLEAR(SFR,N) while ((SFR & (1<<N)));        // se mantiene en espera hasta que un bit cambia a 0

#endif /* BIT_IO_H_ */ 