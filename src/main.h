/*
 * main.c
 *
 *  Created on: 3 abr. 2017
 *      Author: Larraitz
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/

#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo.h"

struct pruebaGPS {
	char hora[8];
	char latitud[10];
	char latCoor[1];
	char longitud[10];
	char longCoor[1];
};

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Buffer used for transmission */
//#define BUFFERSIZE                       (COUNTOF(aTxBuffer) - 1)
/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
/* Exported functions ------------------------------------------------------- */

/* Private function prototypes -----------------------------------------------*/
//static void SystemClock_Config(void);
//static void Error_Handler(void);
void SystemClock_Config(void);
void Error_Handler(void);

#endif /* __MAIN_H */
