/*************************************************************************
* Programa para leer temperatura y presion de un sensor DHT11, asi como obtener
* las coordenas y hora local desde un modulo GPS y transmitir estos datos a traves 
* de un modulo LORA.
* Created: 7/7/2024 12:34:09 PM
* Author:      Ing. Misael Salazar
* File:        main.c
* Target:      ATMEGA328P
**************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include "config_cpu.h"
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "SYSTICK/systick.h"
#include "UART/uart.h"
#include "LCD/lcd_simplex_serial.h"
#include "DHT11/dht11_async.h"
#include "GPS/gps_module.h"
#include "SERIAL/send_serial.h"

#define COUNT_UARTINT_GPS    10
#define RX_BUFFER_SIZE       120      //tamanio del buffer de recepcion

#define PIN_TX_LORA_SERIAL        PORTB3

#define DDR_LED_BLINK_LORA        DDRB
#define PORT_LED_BLINK_LORA       PORTB
#define PIN_LED_BLINK_LORA        PORTB1

#define DDR_LED_BLINK_GPS         DDRB
#define PORT_LED_BLINK_GPS        PORTB
#define PIN_LED_BLINK_GPS         PORTB2

#define DDR_LED_BLINK_SYSTEM_STATUS         DDRB
#define PORT_LED_SYSTEM_STATUS              PORTB
#define PIN_LED_SYSTEM_STATUS               PORTB4

volatile unsigned long systick_count;
char rx_buffer[RX_BUFFER_SIZE];
volatile unsigned char uartmsg;
void print_lcd_dht11(float temperture, float humidyty);

int main(void)
{    
	// variables para el task de lectura del dht11
	const unsigned char periodo_dht11 = 18;                  // para 18ms
	const unsigned int periodo_exec_read_dht11 = 1000;       // para 500ms
	unsigned long current_count_dht11_exec = 0;              // contador para ejecucion de lectura de dht11
	unsigned long current_count_dht11 = 0;
	unsigned char flag_readinprogress_dht11 = 0;
	
	// variables para el task de envio a modulo lora
	const unsigned int periodo_exec_lora = 2000;       // para 1000ms
	unsigned long current_count_lora_exec = 0;
	
	// variables para el task led blink 250ms
	const unsigned int periodo_exec_ledblink = 250;       // para 250ms
	unsigned long current_count_ledblink_exec = 0;
	
	float temperatureDht = 0.0;
	float humedadDht = 0.0;
		
	struct dataGPS dataGPSread = {0};
	char arrayTemp[6] = {0};	
	char arrayHumedad[6] = {0};  
	char arrayLcdHora[24] = {0};
	char arrayLoraMsj[120] = {0};
		
	char arrayLatitud[22] = {0};
	char arrayLongitud[22] = {0};
	char arrayVelocidad[22] = {0};
	
	BIT_SET(DDR_LED_BLINK_LORA, PIN_LED_BLINK_LORA);
	BIT_CLEAR(PORT_LED_BLINK_LORA, PIN_LED_BLINK_LORA);
	BIT_SET(DDR_LED_BLINK_GPS, PIN_LED_BLINK_GPS);
	BIT_CLEAR(PORT_LED_BLINK_GPS, PIN_LED_BLINK_GPS);
	BIT_SET(DDR_LED_BLINK_SYSTEM_STATUS, PIN_LED_SYSTEM_STATUS);
	BIT_CLEAR(PORT_LED_SYSTEM_STATUS, PIN_LED_SYSTEM_STATUS);
	
	lcd_init();
	lcd_set_cursor(1,1);
	lcd_write_string("Iniciando...");
	_delay_ms(3000);
	lcd_clear();
	
	init_systick();                       // se inicia el contador general del sistema
	uart_init();
	enable_int_uart();
	sei();
	
    while (1) 
    {
		// task lectura de uart respuesta modulo gps
		if (uartmsg == 1)
		{					
			// send_serial_write(&DDRB, &PORTB, PIN_TX_LORA_SERIAL, rx_buffer);
			// send_serial_write(&DDRB, &PORTB, PIN_TX_LORA_SERIAL, "\r\n");				
			if (getCoordenadasGPS(rx_buffer, &dataGPSread) != -1)       // si se realizo una lectura de un formato NMAE GPRMC
			{
				// se valida si hay datos
				if (dataGPSread.horaact[0] != '\0' && dataGPSread.latitud[0] != '\0' && dataGPSread.longitud[0] != '\0' && dataGPSread.speed[0] != '\0')
				{
					sprintf(arrayLcdHora, dataGPSread.horaact);
					sprintf(arrayLatitud, dataGPSread.latitud);
					sprintf(arrayLongitud, dataGPSread.longitud);
					sprintf(arrayVelocidad, dataGPSread.speed);
				}
				TOGGLE_BIT(PORT_LED_BLINK_GPS, PIN_LED_BLINK_GPS);										
			}
			else
			{
				sprintf(arrayLcdHora, "nodata");
				sprintf(arrayLatitud, "nodata");
				sprintf(arrayLongitud, "nodata");
				sprintf(arrayVelocidad, "nodata");				
			}
										
			uartmsg = 0;
			// count_uart_int = 0;			
			// memset(arrayTemp, 0, 6);		
			// memset(arrayHumedad, 0, 6);
			// memset(arrayLoraMsj, 0, 120);							
		}
		
		// task de lectura del dhtll
		if ((systick_count - current_count_dht11_exec >= periodo_exec_read_dht11) || (flag_readinprogress_dht11 == 1))      // si ya pasaron 500ms se realiza la lectura del dht11 o si hay una lectura de dht11 en progreso
		{
			// se realiza el task de lectura de dht11
			if (flag_readinprogress_dht11 == 0)
			{
				startpulse_dht11();                    // envia el pulso de inicio al dht11
				current_count_dht11 = systick_count;   // se inicia el conteo de 20ms
				flag_readinprogress_dht11 = 1;         // flag de lectura de dht11 en progreso en 1
			}
			
			if (systick_count - current_count_dht11 >= periodo_dht11)    // si ya pasaron 18ms, se continua con la lectura del dht11
			{
				// leemos el sensor dht11
				read_dht11(&temperatureDht, &humedadDht);
				// print_lcd_dht11(temperatureDht, humidyty);
				
				flag_readinprogress_dht11 = 0;         // flag de lectura de dht11 en progreso en 0
			}
			
			if (flag_readinprogress_dht11 == 0)         // si ya termino la lectura, guardamos el systick
			{
				current_count_dht11_exec = systick_count;
			}
		}
		
		// task de envio de mensaje a LORA cada 1s
		if (systick_count - current_count_lora_exec >= periodo_exec_lora)		
		{
			dtostrf(temperatureDht, 2, 2, arrayTemp);
			dtostrf(humedadDht, 2, 2, arrayHumedad);
			// se envia el mensaje a traves del modulo LORA
			sprintf(arrayLoraMsj, "{%s:%s:%s:%s:%s}", arrayLatitud, arrayLongitud, arrayVelocidad, arrayTemp, arrayHumedad);
			send_serial_write(&DDRB, &PORTB, PIN_TX_LORA_SERIAL, arrayLoraMsj);       // *(uint8_t*)
			send_serial_write(&DDRB, &PORTB, PIN_TX_LORA_SERIAL, "\r\n");						
			
			// led blink led envio msg lora
			TOGGLE_BIT(PORT_LED_BLINK_LORA, PIN_LED_BLINK_LORA);
			lcd_write_string(arrayLcdHora);
			current_count_lora_exec = systick_count;
		}
		
		// task de led blink status
		if (systick_count - current_count_ledblink_exec >= periodo_exec_ledblink)
		{
			TOGGLE_BIT(PORT_LED_SYSTEM_STATUS, PIN_LED_SYSTEM_STATUS);
			current_count_ledblink_exec = systick_count;
		}
    }
}