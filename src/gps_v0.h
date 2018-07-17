/*
 * gps_v0.h
 *
 *  Created on: 24 abr. 2017
 *      Author: Larraitz
 */

#include "main.h"
#include "display.h"
#include <strings.h>
#include <string.h>


struct datosGPS {
	char hora[8];
	char latitud[10];
	char latCoor[1];
	char longitud[10];
	char longCoor[1];
};

struct datosBuffer {
	char datos[200];
};

struct diaDatos{
	char dia[8];
	struct datosGPS datos;
};

void inicializar_gps(void);
void guardar_coordenadas(char buffer[]);
void obtener_coordenadas();
char* formatoLat(char *h);
char* formatoLong(char *h);
char* formatoHora(char *h);
char* formatoFecha(char *f);
void latitudLongitud(char *lat, char *latC, char *lon, char *lonC);
char *add_checksum(uint8_t* string, int length);
void visualizar_coordenadas(void);
void enviar_coord_lora(void);
