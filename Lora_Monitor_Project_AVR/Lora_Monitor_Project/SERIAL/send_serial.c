/*************************************************************************
* Description: Libreria para envio de mensajes a traves del puerto serie 
* por medio del pin seleccionado
* Author:      Ing. Misael Salazar
* File:        send_serial.c
* Target:      ATMEGA328P
**************************************************************************/

#include "send_serial.h"

/*************************************************************************
Envia un mensaje serial a traves del pin seleccionado
param char* str : mensaje a enviar
return void
*************************************************************************/
void send_serial_write(uint8_t* DDR_SERIAL, uint8_t* PORT_SERIAL, uint8_t PIN_SERIAL, const char* str)
{			
	*DDR_SERIAL |= (1<<PIN_SERIAL);
	*PORT_SERIAL |= (1<<PIN_SERIAL);
	
	while(*str != '\0')                  // recorremos los caracteres
	{
		// iniciamos la transmision
		*PORT_SERIAL &= ~(1<<PIN_SERIAL);
		
		#if SERIAL_RATE_1200 == 1
			_delay_us(SERIAL_CALC_UBRR_16(SERIAL_BAUD_RATE_1200_BPS));		
		#elif SERIAL_RATE_2400 == 1		
			_delay_us(SERIAL_CALC_UBRR_16(SERIAL_BAUD_RATE_2400_BPS));		
		#elif SERIAL_RATE_4800 == 1
			_delay_us(SERIAL_CALC_UBRR_16(SERIAL_BAUD_RATE_4800_BPS));		
		#elif SERIAL_RATE_9600 == 1
			_delay_us(SERIAL_CALC_UBRR_16(SERIAL_BAUD_RATE_9600_BPS));		
		#elif SERIAL_RATE_19200 == 1 	
			_delay_us(SERIAL_CALC_UBRR_16(SERIAL_BAUD_RATE_19200_BPS));		
		#elif SERIAL_RATE_28800 == 1
			_delay_us(SERIAL_CALC_UBRR_16(SERIAL_BAUD_RATE_28800_BPS));		
		#elif SERIAL_RATE_38400 == 1
			_delay_us(SERIAL_CALC_UBRR_16(SERIAL_BAUD_RATE_38400_BPS));		
		#elif SERIAL_RATE_57600 == 1
			_delay_us(SERIAL_CALC_UBRR_16(SERIAL_BAUD_RATE_57600_BPS));		
		#elif SERIAL_RATE_76800 == 1
			_delay_us(SERIAL_CALC_UBRR_8(SERIAL_BAUD_RATE_76800_BPS));		
		#elif SERIAL_RATE_115200 == 1		
			_delay_us(CALC_UBRR_8(SERIAL_BAUD_RATE_115200_BPS));		
		#endif
		
		// _delay_us(104);			
		for (uint8_t i = 0; i < 8; i++)           //iniciamos por el bit menos significativo
		{			
			if (*str & (1<<i))
			{
				*PORT_SERIAL |= (1<<PIN_SERIAL);
			}
			else
			{
				*PORT_SERIAL &= ~(1<<PIN_SERIAL);
			}
				
			// _delay_us(104);
			#if SERIAL_RATE_1200 == 1
				_delay_us(SERIAL_CALC_UBRR_16(SERIAL_BAUD_RATE_1200_BPS));
			#elif SERIAL_RATE_2400 == 1
				_delay_us(SERIAL_CALC_UBRR_16(SERIAL_BAUD_RATE_2400_BPS));
			#elif SERIAL_RATE_4800 == 1
				_delay_us(SERIAL_CALC_UBRR_16(SERIAL_BAUD_RATE_4800_BPS));
			#elif SERIAL_RATE_9600 == 1
				_delay_us(SERIAL_CALC_UBRR_16(SERIAL_BAUD_RATE_9600_BPS));
			#elif SERIAL_RATE_19200 == 1
				_delay_us(SERIAL_CALC_UBRR_16(SERIAL_BAUD_RATE_19200_BPS));
			#elif SERIAL_RATE_28800 == 1
				_delay_us(SERIAL_CALC_UBRR_16(SERIAL_BAUD_RATE_28800_BPS));
			#elif SERIAL_RATE_38400 == 1
				_delay_us(SERIAL_CALC_UBRR_16(SERIAL_BAUD_RATE_38400_BPS));
			#elif SERIAL_RATE_57600 == 1
				_delay_us(SERIAL_CALC_UBRR_16(SERIAL_BAUD_RATE_57600_BPS));
			#elif SERIAL_RATE_76800 == 1
				_delay_us(SERIAL_CALC_UBRR_8(SERIAL_BAUD_RATE_76800_BPS));
			#elif SERIAL_RATE_115200 == 1
				_delay_us(CALC_UBRR_8(SERIAL_BAUD_RATE_115200_BPS));
			#endif
		}
					
		// stop bit
		*PORT_SERIAL |= (1<<PIN_SERIAL);
		// _delay_us(104);
		
		#if SERIAL_RATE_1200 == 1
			_delay_us(SERIAL_CALC_UBRR_16(SERIAL_BAUD_RATE_1200_BPS));
		#elif SERIAL_RATE_2400 == 1
			_delay_us(SERIAL_CALC_UBRR_16(SERIAL_BAUD_RATE_2400_BPS));
		#elif SERIAL_RATE_4800 == 1
			_delay_us(SERIAL_CALC_UBRR_16(SERIAL_BAUD_RATE_4800_BPS));
		#elif SERIAL_RATE_9600 == 1
			_delay_us(SERIAL_CALC_UBRR_16(SERIAL_BAUD_RATE_9600_BPS));
		#elif SERIAL_RATE_19200 == 1
			_delay_us(SERIAL_CALC_UBRR_16(SERIAL_BAUD_RATE_19200_BPS));
		#elif SERIAL_RATE_28800 == 1
			_delay_us(SERIAL_CALC_UBRR_16(SERIAL_BAUD_RATE_28800_BPS));
		#elif SERIAL_RATE_38400 == 1
			_delay_us(SERIAL_CALC_UBRR_16(SERIAL_BAUD_RATE_38400_BPS));
		#elif SERIAL_RATE_57600 == 1
			_delay_us(SERIAL_CALC_UBRR_16(SERIAL_BAUD_RATE_57600_BPS));
		#elif SERIAL_RATE_76800 == 1
			_delay_us(SERIAL_CALC_UBRR_8(SERIAL_BAUD_RATE_76800_BPS));
		#elif SERIAL_RATE_115200 == 1
			_delay_us(CALC_UBRR_8(SERIAL_BAUD_RATE_115200_BPS));
		#endif
		str++;
	}
}