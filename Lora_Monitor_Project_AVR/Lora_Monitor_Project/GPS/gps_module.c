/*************************************************************************
 * Description: Libreria para el control del modulo gps neo 6m
 * Author:      Ing. Misael Salazar
 * File:        gps_module.c
 * Created: 07/02/2023 01:59:34 p. m.
 * Target:      ATMEGA
 **************************************************************************/
 
#include "gps_module.h"

static int indexOfGPS(char* strcadena, char* strsearched);
static void getSubstringGPS(char* strcadena, char* strresultado, unsigned char startIndex);
static void normalizaCoordenadas(char* ptrstrcoordenadas, char* ptreastwest);
static void normalizaCoordenadas_v1(char* ptrstrcoordenadas, char* ptreastwest);
static void normalizaHoraUTC(char* strhorautc);
static void utc_to_local_time(char* strhorautc);

/*************************************************************************
Retorna un objeto datagps con la hora, latitud, longitud y velocidad
param char* serialbuffer
param struct dataGPS*
return void
*************************************************************************/
unsigned char getCoordenadasGPS(char* serialbuffer, struct dataGPS* datagps)
{
	char arrayGPRMC[ARRAYGPRMCSIZE] = {0};
	int indiceGPRMC = 0;
	unsigned char indiceSplitComa = 0;
	char* splitArray = "";
	
    // memset(arrayGPRMC, 0, sizeof(arrayGPRMC));
	indiceGPRMC = indexOfGPS(serialbuffer, "GPRMC");
	
	if (indiceGPRMC != -1)                                                 // si encontro la cadena GPRMC, continua
	{
		getSubstringGPS(serialbuffer, arrayGPRMC, indiceGPRMC);            // GPRMC,225446,A,4916.45,N,12311.12,W,000.5
		splitArray = strtok(arrayGPRMC, ",");
		while (splitArray != NULL)
		{
			switch (indiceSplitComa)
			{
				case INDICEHORA :
					strcpy(datagps->horaact, splitArray);
				break;
				case INDICELAT :
					strcpy(datagps->latitud, splitArray);
				break;
				case INDICELON :
					strcpy(datagps->longitud, splitArray);
				break;
				case INDICEEASTWEST :
					strcpy(datagps->eastwest, splitArray);
				break;
				case INDICESPEED :
					strcpy(datagps->speed, splitArray);
				break;
			}
			
			splitArray = strtok(NULL, ",");
			indiceSplitComa++;
		}
		
		if (strlen(datagps->latitud) > 0 && strlen(datagps->longitud) > 0 && strlen(datagps->eastwest) > 0 && strlen(datagps->horaact) > 0)
		{
			normalizaCoordenadas_v1(datagps->latitud, "");
			normalizaCoordenadas_v1(datagps->longitud, datagps->eastwest);
			normalizaHoraUTC(datagps->horaact);
		}
		else
		{
			sprintf(datagps->latitud, "SINDATOS");
			sprintf(datagps->longitud, "SINDATOS");
			sprintf(datagps->horaact, "SINDATOS");
			indiceGPRMC = -1;
		}		
	}
	
	return indiceGPRMC;
}

/*************************************************************************
Regresa el indice de la primera aparicion de la cadena str
param char* strcadena
param char* strsearched
return void
*************************************************************************/        
static int indexOfGPS(char* strcadena, char* strsearched)
{
	char* ptrresult = NULL;
	int indexresult = -1;
	
	ptrresult = strstr(strcadena, strsearched);
	if (ptrresult != NULL)
	{
		indexresult = ptrresult - strcadena;
	}
	
	return indexresult;
}

/*************************************************************************
Normaliza las coordenadas, recorre el punto a la izquierda.
10723.2692 -> 107.232692
param char* strcoordenadas
return void
*************************************************************************/
void normalizaCoordenadas(char* ptrstrcoordenadas, char* ptreastwest)
{
	char* initptrstrcoordenadas = NULL;
	char previusvalue = 0;
	char currentvalue = 0;
	unsigned char contadorciclo = 0;
	unsigned char flagpasopunto = 0;
	unsigned char flagpasosecondpunto = 0;
	char arraystrhorautcsigned[12] = {0};
	
	initptrstrcoordenadas = ptrstrcoordenadas;
	while (*ptrstrcoordenadas != '\0')
	{
		if (contadorciclo == 2)
		{
			previusvalue = *ptrstrcoordenadas;
			*ptrstrcoordenadas = '.';
			flagpasopunto = 1;
			contadorciclo++;
			ptrstrcoordenadas++;
			continue;
		}
		
		if (flagpasopunto == 1)
		{
			if (*ptrstrcoordenadas == '.')
			{
				*ptrstrcoordenadas = previusvalue;
				ptrstrcoordenadas++;
				flagpasosecondpunto = 1;
				continue;
			}
			
			if (flagpasosecondpunto != 1)
			{
				currentvalue = *ptrstrcoordenadas;
				*ptrstrcoordenadas = previusvalue;
			}
		}
		
		previusvalue = currentvalue;
		contadorciclo++;
		ptrstrcoordenadas++;
	}
	
	if (*ptreastwest == 'W')
	{
		ptrstrcoordenadas = initptrstrcoordenadas;        // se vuelve a apuntar a la posicion inicial
		contadorciclo = 0;
		// se agrega signo - a ptrstrcoordenadas
		arraystrhorautcsigned[0] = '-';
		strcat(arraystrhorautcsigned, ptrstrcoordenadas);
		strcpy(ptrstrcoordenadas, arraystrhorautcsigned);
	}
}

/*************************************************************************
Normaliza las coordenadas, recorre el punto a la izquierda.
10723.2692 -> 107.232692
param char* strcoordenadas
return void
*************************************************************************/
void normalizaCoordenadas_v1(char* ptrstrcoordenadas, char* ptreastwest)
{	
	char arraycoordenadas[15] = {0};
	char arraycoordenadasFinal[15] = {0};
	char arraystrhorautcsigned[12] = {0};
	unsigned char contadorIndex = 0;
	unsigned char flagpasopunto = 0;	

	strcpy(arraycoordenadas, ptrstrcoordenadas);
	for (unsigned char indexArr = 0; indexArr <= strlen(arraycoordenadas); indexArr++)           // 10724.24991    1072424991
	{
		if (arraycoordenadas[indexArr] != '.')
		{
			arraycoordenadasFinal[contadorIndex] = arraycoordenadas[indexArr];
			contadorIndex++;
		}
	}

	strcpy(arraycoordenadas, arraycoordenadasFinal);
	memset(arraycoordenadasFinal, 0, sizeof(arraycoordenadasFinal));
	contadorIndex = 0;

	char indexArr = strlen(arraycoordenadas) - 1;
	while (indexArr >= -1)
	{
		if (contadorIndex != 7)
		{
			if (flagpasopunto == 0)
			{
				arraycoordenadasFinal[indexArr + 1] = arraycoordenadas[indexArr];
			}
			else
			{
				arraycoordenadasFinal[indexArr + 1] = arraycoordenadas[indexArr + 1];
			}
		}
		else
		{
			arraycoordenadasFinal[indexArr + 1] = '.';
			flagpasopunto = 1;
		}

		contadorIndex++;
		indexArr--;
	}

	strcpy(ptrstrcoordenadas, arraycoordenadasFinal);
	if (*ptreastwest == 'W')
	{
		// se agrega signo - a ptrstrcoordenadas
		arraystrhorautcsigned[0] = '-';
		strcat(arraystrhorautcsigned, ptrstrcoordenadas);
		strcpy(ptrstrcoordenadas, arraystrhorautcsigned);
	}
}

/*************************************************************************
Normaliza las hora UTC agregando :
161229 -> 16:12:29
param char* strhorautc
return void
*************************************************************************/
void normalizaHoraUTC(char* strhorautc)
{
	char* initptrstrhorautc = NULL;
	unsigned char contadorciclo = 0;
	unsigned char contadorpunto = 0;
	char arraystrhorautc[9] = {0};
	
	initptrstrhorautc = strhorautc;
	while (*strhorautc != '.')
	{
		if (contadorpunto %2 == 0 && contadorpunto > 0)
		{
			arraystrhorautc[contadorciclo] = ':';
			contadorpunto = 0;
			contadorciclo++;
			continue;
		}
		else
		{
			arraystrhorautc[contadorciclo] = *strhorautc;
			strhorautc++;
		}
		contadorpunto++;
		contadorciclo++;
	}
	
	strhorautc = initptrstrhorautc;           // se vuelve a apuntar al inicio del string
	strcpy(strhorautc, arraystrhorautc);
	utc_to_local_time(strhorautc);
}

/*************************************************************************
Convierte de UTC a hora local
param char* strhorautc
return void
*************************************************************************/
void utc_to_local_time(char* strhorautc)
{
	char* initptrstrhorautc = NULL;
	char arrayhorautc[3] = {0};
	char horautc = 0;

	initptrstrhorautc = strhorautc;
	arrayhorautc[0] = *strhorautc;
	strhorautc++;
	arrayhorautc[1] = *strhorautc;
	horautc = atoi(arrayhorautc);
	horautc -= UTC_TO_LOCAL;
	if (horautc <= 0)
	{
		horautc = atoi(arrayhorautc);
		horautc = (horautc - UTC_TO_LOCAL) + 24;
	}
	
	sprintf(arrayhorautc, "%02d", horautc);
	strhorautc = initptrstrhorautc;            // se reinicia el puntero
	*strhorautc = arrayhorautc[0];
	strhorautc++;
	*strhorautc = arrayhorautc[1];
	strhorautc = initptrstrhorautc;            // se reinicia el puntero
}

/*************************************************************************
Retorna el substring entre el numero de comas indicado
param char* strcadena
param char* strresultado
param
return void
*************************************************************************/
static void getSubstringGPS(char* strcadena, char* strresultado, unsigned char startIndex)
{
	char* ptrcadena = NULL;
	char* ptresultado = NULL;
	unsigned char contadorComa = 0;
	
	ptrcadena = strcadena + startIndex;
	ptresultado = strresultado;
	
	while (contadorComa < COUNTCOMA)
	{		
	   *ptresultado = *ptrcadena++;
		ptresultado++;

		if (*ptrcadena == ',')
		{
			contadorComa++;
		}					
	}
}















































