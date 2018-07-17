/*
 * bateria.c
 *
 *  Created on: 20 abr. 2017
 *      Author: Larraitz
 */

int filas = 160;
int columnas = 5;

int matriz_0[160][5];
int matriz_1[160][5];
int matriz_2[160][5];
int matriz_3[160][5];

unsigned char bat0[] = { 0x0E, 0x1B, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1F};
unsigned char bat1[] = { 0x0E, 0x1B, 0x11, 0x11, 0x11, 0x11, 0x1F, 0x1F};
unsigned char bat2[] = { 0x0E, 0x1B, 0x11, 0x11, 0x11, 0x1F, 0x1F, 0x1F};
unsigned char bat3[] = { 0x0E, 0x1B, 0x11, 0x11, 0x1F, 0x1F, 0x1F, 0x1F};
unsigned char bat4[] = { 0x0E, 0x1B, 0x11, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F};
unsigned char bat5[] = { 0x0E, 0x1B, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F};
unsigned char bat6[] = { 0x0E, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F};


/**
  * @brief  Se crean las matrices con los valores de la bateria
  * @param  None
  * @retval None
  */
void crear_matrices(void)
{
	int z = 0;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 160; j++) {
			matriz_0[j][i] = z;
			z++;
		}
	}
	z = 801;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 160; j++) {
			matriz_1[j][i] = z;
			z++;
		}
	}
	z = 1601;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 160; j++) {
			matriz_2[j][i] = z;
			z++;
		}
	}
	z = 2401;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 160; j++) {
			matriz_3[j][i] = z;
			z++;
		}
	}
}

/**
  * @brief  Se busca el valor de la tension en las matrices
  * @param  x: valor de la tension, numMatr: numero de la matriz,
  * 		col: numero de columna, fila: numero de fila
  * @retval None
  */
int buscarvalor(int x, int *numMatr, int *col, int *fila)
{
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 160; j++) {
			if (matriz_0[j][i] == x)
			{
				*numMatr = 0;
				*col = i;
				*fila = j;
				break;
			}
		}
	}
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 160; j++) {
			if (matriz_1[j][i] == x)
			{
				*numMatr = 1;
				*col = i;
				*fila = j;
				break;
			}
		}
	}
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 160; j++) {
			if (matriz_2[j][i] == x)
			{
				*numMatr = 2;
				*col = i;
				*fila = j;
				break;
			}
		}
	}
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 160; j++) {
			if (matriz_3[j][i] == x)
			{
				*numMatr = 3;
				*col = i;
				*fila = j;
				break;
			}
		}
	}
	return 0;
}
