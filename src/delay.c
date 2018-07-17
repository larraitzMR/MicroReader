/*
 * delay.c
 *
 *  Created on: 5 abr. 2017
 *      Author: Propietario
 */

#include "main.h"

/**
  * @brief	Inicializacion del delay
  * @param  None
  * @retval None
  */
void delay_Init(void) {
// 1 milisecond per tick
// SysTick_Config ( SystemCoreClock / 1000); // notacion CMSIS
	HAL_SYSTICK_Config(SystemCoreClock / 1000);
}

/**
  * @brief  Delay de milisegundos
  * @param  None
  * @retval None
  */
void delay_ms(uint32_t miliseconds) {
	uint32_t ticks_end;
	ticks_end = HAL_GetTick() + miliseconds;
	while (HAL_GetTick() < ticks_end) {};
}
