/*
 * timer.c
 *
 *  Created on: 12 may. 2017
 *      Author: Larraitz
 */

#include "main.h"
#include "timer.h"

TIM_HandleTypeDef htim2;
TIM_ClockConfigTypeDef sClockSourceConfig;

void TIM2_Config(void)
{
    __TIM2_CLK_ENABLE();

//	htim2.Init.Prescaler = (SystemCoreClock/1000) - 1;
	htim2.Init.Prescaler = 840000;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 10000 - 1;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.RepetitionCounter = 0;
	htim2.Instance = TIM2;
	HAL_TIM_Base_Init(&htim2);
	HAL_TIM_Base_Start_IT(&htim2);

    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);



}

void TIM2_Init(void)
{
    /* Peripheral clock enable */

  /* Peripheral interrupt init*/

}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    __TIM3_CLK_DISABLE();

    /* Peripheral interrupt DeInit*/
    HAL_NVIC_DisableIRQ(TIM3_IRQn);
	enviar_coord_lora();
}
