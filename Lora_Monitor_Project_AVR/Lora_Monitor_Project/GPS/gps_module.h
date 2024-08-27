/*************************************************************************
 * Description: Libreria para el control del modulo gps neo 6m
 * Author:      Ing. Misael Salazar
 * File:        gps_module.h
 * Created: 07/02/2023 01:59:34 p. m.
 * Target:      ATMEGA
 **************************************************************************/
 
#ifndef GPS_MODULE_H_
#define GPS_MODULE_H_
 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define COUNTCOMA      8
#define INDICEHORA     1
#define INDICELAT      3
#define INDICELON      5
#define INDICEEASTWEST 6
#define INDICESPEED    7
#define ARRAYGPRMCSIZE 80
#define UTC_TO_LOCAL   8

struct dataGPS
{
	char horaact[20];
	char latitud[20];
	char longitud[20];
	char eastwest[3];
	char speed[20];
};

/**
 @brief Lee la trama recibida por el neo 6m
 @param char* serialbuffer
 @param struct dataGPS*
 @return unsigned char
 */
unsigned char getCoordenadasGPS(char* serialbuffer, struct dataGPS* datagps);

#endif /* GPS_MODULE_H_ */