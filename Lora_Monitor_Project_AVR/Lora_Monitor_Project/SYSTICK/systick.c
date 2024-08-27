/*************************************************************************
* Description: Libreria para la configuracion del contador general del sistema
* Author:      Ing. Misael Salazar
* File:        systick.h
* Target:      ATMEGA328P
**************************************************************************/

#include "systick.h"

// Handler de interrupcion TIMER1_COMPA
ISR(TIMER1_COMPA_vect)
{
	systick_count++;
}

static void set_preescaler(unsigned int preescaler);

/*************************************************************************
Funcion init_systick, inicia el systick
*************************************************************************/
void init_systick()
{
	// Operation mode CTC mode 4
    TCCR1A &=~ (1<<WGM10);
	TCCR1A &=~ (1<<WGM11);
	TCCR1B |=  (1<<WGM12);
	TCCR1B &=~ (1<<WGM13);
	
	OCR1A = CARGATIMER;              // se carga el timer para (2000) 1ms
	set_preescaler(PREESCALER_8);    // se configura el preescaler
	TIMSK1 |= (1<<OCIE1A);           // Se configura la interrupcion por output compare match A
}

/*************************************************************************
Funcion set_preescaler, configura el preescaler para el timer0
*************************************************************************/
static void set_preescaler(unsigned int preescaler)
{
	switch (preescaler)
	{
		case PREESCALER_1 :           //preescaler en 1
		{
			TCCR1B |=  (1<<CS10);
	        TCCR1B &=~ (1<<CS11);
	        TCCR1B &=~ (1<<CS12);
		}
		break;
		case PREESCALER_8 :           //preescaler en 8
		{
			TCCR1B &=~ (1<<CS10);
	        TCCR1B |=  (1<<CS11);
	        TCCR1B &=~ (1<<CS12);
		}
		break;
		case PREESCALER_64 :          //preescaler en 64
		{
			TCCR1B |=  (1<<CS10);
	        TCCR1B |=  (1<<CS11);
	        TCCR1B &=~ (1<<CS12);
		}
		break;
		case PREESCALER_256 :         //preescaler en 256
		{
			TCCR1B &=~ (1<<CS10);
	        TCCR1B &=~ (1<<CS11);
	        TCCR1B |=  (1<<CS12);
		}
		break;
		case PREESCALER_1024 :       //preescaler en 1024
		{
			TCCR1B |=  (1<<CS10);
	        TCCR1B &=~ (1<<CS11);
	        TCCR1B |=  (1<<CS12);
		}
		break;
	}
}

/*************************************************************************
Funcion systick_off, apaga el systick
*************************************************************************/
void systick_off()
{
	TCCR1B &=~ (1<<CS10);
	TCCR1B &=~ (1<<CS11);
	TCCR1B &=~ (1<<CS12);
}





































