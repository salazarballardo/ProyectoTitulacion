/*************************************************************************
* Description: Libreria para lectura de temperatura y presion con el sensor DHT11
*              de forma asincrona (sin el delay de 18ms)
* Author:      Ing. Misael Salazar
* File:        dht11_async.h
* Target:      ATMEGA328P
**************************************************************************/

#include "dht11_async.h"

// funciones estaticas o locales
static unsigned char readstart_dht11();
static void readByte(unsigned char* data);
static inline void timer0Stop();
static inline void timer0Start();

unsigned char errorRead;

/*************************************************************************
Funcion startpulse_dht11
Envia el pulso de inicio al dht11
param void
return void
*************************************************************************/
void startpulse_dht11()
{
	// envia pulso start
	errorRead = 0;
	BIT_SET(DDRPORTDHT, PINDATADHT);           //lo configuramos como salida
	BIT_CLEAR(PORTDATADHT, PINDATADHT);
}

/*************************************************************************
Funcion readstart_dht11
iniciamos el DHT11
Retorna un 1 si es exitoso, 0 si hay error
param void
return unsigned char error
*************************************************************************/
static unsigned char readstart_dht11()
{
	BIT_SET(PORTDATADHT, PINDATADHT);
	
	_delay_us(30);          // wait 30us
	BIT_CLEAR(DDRPORTDHT, PINDATADHT);
	
	timer0Start();
	while (!BIT_TEST(PORDATAREAD, PINDATADHT) && TCNT0 < 200);  //mientras estemos en cero y no pase de 90us
	//valueTimer0 = TCNT0;
	
	if (TCNT0 < 200)
	{
		timer0Stop();
		timer0Start();
		while (BIT_TEST(PORDATAREAD, PINDATADHT) && TCNT0 < 200);  //mientras estemos en uno y no pase de 90us
		//valueTimer0 = TCNT0;
		
		if (TCNT0 < 200)
		{
			timer0Stop();
			return 1; 
		}
		else
		{
			timer0Stop();
			return 0;
		}
	}
	else
	{
		timer0Stop();
		return 0;
	}
}

/*************************************************************************
Funcion read_dht11
Funcion principal que lanza la lectura de presion y temperatura del dht11
param float* temperature : retorna la temperatura por referencia
param float* humidity : retorna la humedad por referencia
return unsigned char : retorna un 1 en caso de exito de lectura o cero si hay error
*************************************************************************/
unsigned char read_dht11(float* temperature, float* humidity)
{
	unsigned char humidyHigh = 0;
	unsigned char humidyLow = 0;
	unsigned char tempHigh = 0;
	unsigned char tempLow = 0;
	unsigned char checksum = 0;
	
	if (readstart_dht11() == 0)                    // iniciamos el sensor
	{
		return 0;                 // ocurrio un error al inicializar el sensor, salimos
	}
	
	// leemos los paquetes de 5 bytes
	for (unsigned char l = 0; l < 5; l++)
	{
		switch (l)
		{
			case 0:
			{
				readByte(&humidyHigh);
				if (errorRead == 1)           //si ocurrio un error salimos
				{
					return 0;
				}
			}
				break;
			case 1:
			{
				readByte(&humidyLow);
				if (errorRead == 1)           //si ocurrio un error salimos
				{
					return 0;
				}
			}
				break;
			case 2:
			{
				readByte(&tempHigh);
				if (errorRead == 1)           //si ocurrio un error salimos
				{
					return 0;
				}
			}
				break;
			case 3:
			{
				readByte(&tempLow);
				if (errorRead == 1)           //si ocurrio un error salimos
				{
					return 0;
				}
			}
				break;
			case 4:
			{
				readByte(&checksum);
				if (errorRead == 1)           //si ocurrio un error salimos
				{
					return 0;
				}
			}
			break;
		}
	}
	
	//armamos las cifras de temp y humedad 
	*temperature = (float)(tempHigh + ((float)tempLow / 100));
	*humidity = (float)(humidyHigh + ((float)humidyLow / 100));
	
	return 1;
}

/*************************************************************************
Funcion readByte
Se lee el byte correspondiente retornado por el dht11
param unsigned char* data : retorna el byte leido por referencia
return void
*************************************************************************/
static void readByte(unsigned char* data)
{
    unsigned char countBit = 0;
	
	while (countBit < 8)
	{
		//leemos el pin
		timer0Start();
		while (!BIT_TEST(PORDATAREAD, PINDATADHT) && TCNT0 < 141);  //mientras estemos en cero y no pase de 70us
		
		if (TCNT0 > 150)
		{
			errorRead = 1;
			timer0Stop();
			break;
		}
		else
		{
			timer0Stop();
			timer0Start();
			while (BIT_TEST(PORDATAREAD, PINDATADHT) && TCNT0 < 151);  //mientras estemos en uno y no pase de 75us en nivel alto
			
			if (TCNT0 > 170)
			{
				errorRead = 1;
				timer0Stop();
				break;
			}
			else
			{
				if (TCNT0 > 65)             //si el pulso fue mayor a 30us es 1
				{
					*data |= (1<<(7 - countBit));
				}
				
				timer0Stop();
			}
		}
		
		countBit++;
	}
}

/*************************************************************************
Funcion timer0Start
Inicia el conteo del timer0
param void
return void
*************************************************************************/
static inline void timer0Start()
{
	TCCR0B &= ~(1<<CS00);  //preescaler 8
	TCCR0B |= (1<<CS01);
	TCCR0B &= ~(1<<CS02);
	//TCNT0 = 0;
}

/*************************************************************************
Funcion timer0Stop
Detiene el conteo del timer0
param void
return void
*************************************************************************/
static inline void timer0Stop()
{
	TCCR0B &= ~(1<<CS00);
	TCCR0B &= ~(1<<CS01);
	TCCR0B &= ~(1<<CS02);
	TCNT0 = 0;
}





























