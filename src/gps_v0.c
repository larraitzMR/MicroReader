/*
 * gps_v0.c
 *
 *  Created on: 24 abr. 2017
 *      Author: Larraitz
 */

#include "gps_v0.h"
#include "main.h"
#include "display.h"
#include "spi.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *nmea[15];
int i = 0;
int numGPS = 0;
int z = 0;
char *hora, *dia, *latitud, *latCoor, *longitud, *longCoor, *fecha;
char *tipo;
int c = 0;
int mb = 0;
int dat = 0;
int num;
int fechaOK = 0;

/* Private functions ---------------------------------------------------------*/
uint8_t init_nmea[] = "$PSRF100,0,9600,8,1,0*04\r\n";
uint8_t init_GGA[] = "$PSRF103,00,00,01,01*25\r\n";
uint8_t deinit_GGA[] = "$PSRF103,00,00,00,01*25\r\n";
uint8_t init_GLL[] = "$PSRF103,01,00,01,01*24\r\n";
uint8_t deinit_GLL[] = "$PSRF103,01,00,00,01*25\r\n";
uint8_t init_GSA[] = "$PSRF103,02,00,01,01*27\r\n";
uint8_t deinit_GSA[] = "$PSRF103,02,00,00,01*26\r\n";
uint8_t init_GSV[] = "$PSRF103,03,00,05,01*22\r\n";
uint8_t deinit_GSV[] = "$PSRF103,03,00,00,01*27\r\n";
uint8_t init_RMC[] = "$PSRF103,04,00,01,01*21\r\n";
uint8_t deinit_RMC[] = "$PSRF103,04,00,00,01*20\r\n";
uint8_t init_VTG[] = "$PSRF103,05,00,01,01*20\r\n";
uint8_t deinit_VTG[] = "$PSRF103,05,00,00,01*21\r\n";

//extern char RxBuf[];
#define RBUFFSIZE 77
char reciboGPS[RBUFFSIZE];

char bufferReady[5];
char buffer[10];
char bufferGPS[40];
char bufferReceive[2];
char bufferParsing[40];
int len;
extern UART_HandleTypeDef huart6;
extern UART_HandleTypeDef huart2;
extern SPI_HandleTypeDef hspi2;

//struct datosGPS miGPS[1000];
struct datosBuffer mibuffer[200];
struct diaDatos GPS[1000];

__IO ITStatus UartReady = RESET;

void inicializar_gps(void)
{
	if (HAL_UART_Receive_DMA(&huart6, (uint8_t *) reciboGPS, 200) != HAL_OK) {
		Error_Handler();
	}

	if (HAL_UART_Transmit_DMA(&huart6, (uint8_t *)deinit_GSV, sizeof(deinit_GSV))!= HAL_OK) {
		Error_Handler();
	}
	while (UartReady != SET) {
	}
	UartReady = RESET;

	if (HAL_UART_Transmit_DMA(&huart6, (uint8_t *)deinit_GSA, sizeof(deinit_GSA))!= HAL_OK) {
		Error_Handler();
	}
	while (UartReady != SET) {
	}
	UartReady = RESET;

	if (HAL_UART_Transmit_DMA(&huart6, (uint8_t *)deinit_GGA, sizeof(deinit_GGA))!= HAL_OK) {
		Error_Handler();
	}
	while (UartReady != SET) {
	}
	UartReady = RESET;

	if (HAL_UART_Transmit_DMA(&huart6, (uint8_t *)init_RMC, sizeof(init_RMC))!= HAL_OK) {
		Error_Handler();
	}
	while (UartReady != SET) {
	}
	UartReady = RESET;
}


/**
  * @brief  Las tramas obtenidas se guardan en un structs
  * @param  None
  * @retval None
  */
void guardar_coordenadas(char buffer[])
{
	strcpy(mibuffer[c].datos, buffer);
	c++;
	if (c == 199)
	{
		c = 0;
	}
	obtener_coordenadas();
}

/**
  * @brief  Se obtienen las coordenadas del gps por la uart
  * @param  None
  * @retval None
  */
void obtener_coordenadas() {

	char* str = NULL;
	char* car = NULL;
	char* p;
//	char* busca = "$GPGGA";
	char* busca = "$GNRMC";

	str = strstr(mibuffer[mb].datos,busca);

	if (str)
	{
		car = strchr(str,'\n');
		if (car){
			mb++;
		}
		else {
			str = NULL;
			mb++;
			if (mb == 199){
				mb = 0;
			}
			return;
		}

	} else {
		str = NULL;
		mb++;
		if (mb == 199){
			mb = 0;
		}
		return;
	}

	if (mb == 199){
		mb = 0;
	}

	for (p = strtok(str + 1, ","); p; p = strtok( NULL, ",")) {
		nmea[i] = p;
		i++;
		if (i == 13){break;}
	}
	p = NULL;
	i = 0;
	tipo = *&nmea[0];
	// http://www.gpsinformation.org/dale/nmea.htm#GGA
	// Example GNRMC
	// Array index -->        0   |      1      |    2    |     3     | 4 |     5      | 6 |   7  |   8    |    9   |10|11|  12  | 13  |   14
	// Example     -->     $GNRMC ,  014035.00 	,    A    , 4317.3578 , N , 00159.1794 , W , 0.00 , 100.74 , 060417 ,  ,  , A*67 ,
	// Description -->            |  Time UTC  	|  status | latitude  |N/S| longitude  |W/E|Speed |  Angle |  Date	|  |  |Checksum
	//                              01:40:35 UTC| A=active
	//                                          |  V=Void

	if (strcmp(tipo, "GNRMC") == 0) {
		if ((fechaOK == 0) && (*&nmea[9] != '\0')) {
			fecha = *&nmea[9];
			fecha = formatoFecha(fecha);
			strcpy(GPS[numGPS].dia, fecha);
			fechaOK = numGPS;
		}

		hora = *&nmea[1];
		hora = formatoHora(hora);
		latitud = *&nmea[3];
		latitud = formatoLat(latitud);
		latCoor = *&nmea[4];
		longitud = *&nmea[5];
		longitud = formatoLong(longitud);
		longCoor = *&nmea[6];

		strcpy(GPS[numGPS].datos.hora, hora);
		strcpy(GPS[numGPS].datos.latitud, latitud);
		strcpy(GPS[numGPS].datos.latCoor, latCoor);
		strcpy(GPS[numGPS].datos.longitud, longitud);
		strcpy(GPS[numGPS].datos.longCoor, longCoor);

		numGPS++;
		if (numGPS == 999) {
			numGPS = 0;
		}
	}

// http://www.gpsinformation.org/dale/nmea.htm#GGA
// Example GPGGA
// Array index -->        0   |      1       |     2     | 3 |    4       | 5 |     6     |       7     |     8     |    9   |  10  |     11	 |  12  |13|   14
// Example     -->     $GPGGA ,  092152.000  , 4317.3578 , N , 00159.1794 , W ,     1	  ,		06	    ,    1.5    , -10.4  ,   M  ,    51.0	 ,   M  ,  , 0000*54
// Description -->            |   Time UTC   | Latitude  |N/S| Longitude  |W/E|Fix quality|N. satellites|H.diluition|Altitude|Meters|Heigth geoid|Meters|  |checksum
//                              09:21:52 UTC								  |1 = GPS fix
//
	else if (strcmp(tipo, "GPGGA") == 0) {
		hora = *&nmea[1];
		hora = formatoHora(hora);
		latitud = *&nmea[2];
		latitud = formatoLat(latitud);
		latCoor = *&nmea[3];
		longitud = *&nmea[4];
		longitud = formatoLong(longitud);
		longCoor = *&nmea[5];

		strcpy(GPS[numGPS].datos.hora, hora);
		strcpy(GPS[numGPS].datos.latitud, latitud);
		strcpy(GPS[numGPS].datos.latCoor, latCoor);
		strcpy(GPS[numGPS].datos.longitud, longitud);
		strcpy(GPS[numGPS].datos.longCoor, longCoor);

		numGPS++;
		if (numGPS == 999) {
			numGPS = 0;
		}
	}
}


int glora = 0;
int envioReady = 0;
char horaL[9];
char latitudL[11];
char latCoorL[2];
char longitudL[11];
char longCoorL[2];
int reciboOK = 0;
//int reciboReady = 0;
int ready = 0;

uint8_t ReadyMsg[] = "READY";
uint8_t OKMsg[] = "OK";

void enviar_coord_lora(void)
{
	if (ready == 1)
	{
		int numero = numGPS-1;
		if (numero == -1)
		{
			numero = 999;
		}
		strcpy(horaL, GPS[numero].datos.hora);
		horaL[8] = '\0';
		strcpy(latitudL, GPS[numero].datos.latitud);
		latitudL[10] = '\0';
		strcpy(latCoorL, GPS[numero].datos.latCoor);
		latCoorL[1] = '\0';
		strcpy(longitudL, GPS[numero].datos.longitud);
		longitudL[10] = '\0';
		strcpy(longCoorL, GPS[numero].datos.longCoor);
		longCoorL[1] = '\0';

		sprintf(bufferGPS, "GPS %s %s %s %s %s\r\n", horaL, latitudL, latCoorL, longitudL,	longCoorL);
		len = strlen(bufferGPS);
		HAL_UART_Transmit(&huart2, (uint8_t*) bufferGPS, strlen(bufferGPS),1000);

		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
		HAL_SPI_TransmitReceive(&hspi2, (uint8_t*) bufferGPS,(uint8_t*) bufferReceive, len, 2000);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

		if(Buffercmp((uint8_t*)OKMsg, (uint8_t*)bufferReceive, 2)) {
//			HAL_UART_Transmit(&huart2, (uint8_t*) bufferReceive, strlen(bufferReceive),1000);
//			HAL_UART_Transmit(&huart2, (uint8_t*) OKMsg, strlen(OKMsg),1000);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
			HAL_SPI_TransmitReceive(&hspi2, (uint8_t*) OKMsg, (uint8_t*) bufferParsing , len, 2000);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);

			if(Buffercmp((uint8_t*)bufferGPS, (uint8_t*)bufferParsing, 2)) {
				//empezar timer
			    __TIM3_CLK_ENABLE();
			    HAL_NVIC_EnableIRQ(TIM2_IRQn);
			}

		} else {
//			HAL_UART_Transmit(&huart2, (uint8_t*) bufferReceive, strlen(bufferReceive),1000);
			//Hacer una cuenta de intentos y cuando llegue al número de intentos cambiar a GPRS.
			//Numero de intentos = 4
			//Para cambiar a GPRS, desactivar la SPI, activar I2C (cuando funcione) e inicializar el GPRS.
		}
		Flush_Buffer((uint8_t*)bufferReceive, len);
		Flush_Buffer((uint8_t*)bufferParsing, strlen(bufferParsing));

	}
}

char diastr[9];
char horastr[9];
char latitudstr[11];
char latCoorstr[2];
char longitudstr[11];
char longCoorstr[2];
char subbuff[8];

/**
  * @brief  Funcion para visualizar las coordenadas en el LCD
  * @param  None
  * @retval None
  */
void visualizar_coordenadas(void)
{
	int gps = numGPS-1;
	if (gps == -1)
	{
		gps = 999;
	}
	if (strcmp(GPS[gps].datos.hora, '\0') == 0) {
		return;
	} else {
		strcpy(diastr, GPS[fechaOK].dia);
		diastr[8] = '\0';
		strcpy(horastr, GPS[gps].datos.hora);
		horastr[8] = '\0';
		strcpy(latitudstr, GPS[gps].datos.latitud);
		latitudstr[10] = '\0';
		strcpy(latCoorstr, GPS[gps].datos.latCoor);
		latCoorstr[1] = '\0';
		strcpy(longitudstr, GPS[gps].datos.longitud);
		longitudstr[10] = '\0';
		strcpy(longCoorstr, GPS[gps].datos.longCoor);
		longCoorstr[1] = '\0';
		LCD_Cursor(1);
		LCD_Print_String(diastr);
		LCD_Cursor(2);

		LCD_Print_String(horastr);
		LCD_Cursor(3);
		LCD_Print_String("Lat: ");
		LCD_Print_String(latitudstr);
		LCD_Print_String(latCoorstr);
		LCD_Cursor(4);
		LCD_Print_String("Long: ");
		LCD_Print_String(longitudstr);
		LCD_Print_String(longCoorstr);
	}
}


char time[10] = "";
char hora_array[9] = "";
char data[6] = "";
char fecha_array[8] = "";
char lati[11];
char longi[11];
char g[2];
char m[2];
char s[2];
char sd[1];


/**
  * @brief  Se ajusta el formato de latitud
  * @param  h: latitud
  * @retval None
  */
char* formatoLat(char *h) {

	double degValue = atof(h)/100;
	int grados = degValue;
	double decMinutesSeconds = ((degValue - grados)) / 0.60;
	double minuteValue = decMinutesSeconds * 60;
	int minutes = (int) minuteValue;
	double secsValue = (minuteValue - minutes) * 60;
	int segundos = secsValue;
	int decSsegundos = (secsValue-segundos) * 10;

	if(grados < 10){
		sprintf(g, "0%d", grados);
	} else {
		sprintf(g, "%d", grados);
	}
	if(minutes < 10){
		sprintf(m, "0%d", minutes);
	} else {
		sprintf(m, "%d", minutes);
	}
	if(segundos < 10){
		sprintf(s, "0%d", segundos);
	} else {
		sprintf(s, "%d", segundos);
	}
	sprintf(sd, "%d", decSsegundos);

	sprintf(lati, "%s %s %s.%s%s", g,m,s,sd,'\0');

	return lati;
}

/**
  * @brief  Se ajusta el formato de longitud
  * @param  h: longitud
  * @retval None
  */
char* formatoLong(char *h) {

	double degValue = atof(h)/100;
	int grados = degValue;
	double decMinutesSeconds = ((degValue - grados)) / 0.60;
	double minuteValue = decMinutesSeconds * 60;
	int minutes = (int) minuteValue;
	double secsValue = (minuteValue - minutes) * 60;
	int segundos = secsValue;
	int decSsegundos = (secsValue-segundos) * 10;

	if(grados < 10){
		sprintf(g, "0%d", grados);
	} else {
		sprintf(g, "%d", grados);
	}
	if(minutes < 10){
		sprintf(m, "0%d", minutes);
	} else {
		sprintf(m, "%d", minutes);
	}
	if(segundos < 10){
		sprintf(s, "0%d", segundos);
	} else {
		sprintf(s, "%d", segundos);
	}
	sprintf(sd, "%d", decSsegundos);

	sprintf(longi, "%s %s %s.%s%s", g,m,s,sd,'\0');

	return longi;
}

int forhora;
char hora_real[2];
/**
  * @brief  Se ajusta el formato de hora
  * @param  h: hora
  * @retval None
  */
char* formatoHora(char *h) {

	strcpy(time, h);
	memset(&hora_array[0], 0, sizeof(hora_array));
	forhora = atoi(h);
	forhora = forhora/10000;
	forhora = forhora+2;
	if (forhora<10)
	{
		sprintf(hora_real, "0%d", forhora);
	} else if (forhora<24)
	{
		sprintf(hora_real, "%d", forhora);
	} else {
		forhora = forhora-24;
		sprintf(hora_real, "0%d", forhora);
	}

	hora_array[0] = hora_real[0];
	hora_array[1] = hora_real[1];
	hora_array[2] = ':';
	hora_array[3] = time[2];
	hora_array[4] = time[3];
	hora_array[5] = ':';
	hora_array[6] = time[4];
	hora_array[7] = time[5];
	hora_array[8] = '\0';
	return hora_array;
}

/**
  * @brief  Se ajusta el formato de hora y de fecha
  * @param  h: hora, f: fecha
  * @retval None
  */
char* formatoFecha(char *f) {

	strcpy(data, f);
	fecha_array[0] = data[0];
	fecha_array[1] = data[1];
	fecha_array[2] = '/';
	fecha_array[3] = data[2];
	fecha_array[4] = data[3];
	fecha_array[5] = '/';
	fecha_array[6] = data[4];
	fecha_array[7] = data[5];
	return fecha_array;
}

/**
  * @brief  Se ajusta el formato de longitud y latitud
  * @param  lat: latitud, latC: coordenada de la latitud,
  * 		lon: longitud, lonC: coordenada de la longitud
  * @retval None
  */
void latitudLongitud(char *lat, char *latC, char *lon, char *lonC) {

	LCD_Cursor(3);
	LCD_Print("Lat: ");
	LCD_Print(lat);
	LCD_Print(latC);
	LCD_Cursor(4);
	LCD_Print("Long: ");
	LCD_Print(lon);
	LCD_Print(lonC);
}

/**
  * @brief  Se calcula el checksum para la sentencia NMEA
  * @param  string: la sentencia NMEA, length: largura de la sentencia
  * @retval None
  */
char *add_checksum(uint8_t* string, int length)
{
	//Cada caracter del string pasarlo a decimal, hacer XOR entre todos los caracteres y luego a hexadecimal.
	//PSRF105,1 -> 80 83 82 70 49 48 53 44 49.
	char *nmea = malloc(length);
	int checksum = 0;
	int ch = 0;
	char check[2];
//	strcpy(nmea,string);
	ch = 1;
	while (string[ch] != '*'){
		checksum = checksum ^ string[ch];
		ch++;
	}
	nmea[ch] = '*';
	//para pasar a hexadecimal
	sprintf(check, "%x", checksum);
	nmea[ch + 1] = check[0];
	nmea[ch + 2] = check[1];
	nmea[ch + 3] = '\r';
	nmea[ch + 4] = '\n';

	return nmea;
}


