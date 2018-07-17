/*
 * DAC.c
 *
 *  Created on: 29 mar. 2017
 *      Author: Larraitz
 */

#include "main.h"
#include "stm32f4xx_hal_adc.h"

#define ADC1_CLK_ENABLE()               __ADC1_CLK_ENABLE();

/* Definition of ADCx channels */
#define ADC_CHANNEL                   	ADC_CHANNEL_8

/* Definition of ADCx NVIC resources */
#define ADCx_IRQn                       ADC1_COMP_IRQn
#define ADCx_IRQHandler                 ADC1_COMP_IRQHandler

#define GPIOB_CLK_ENABLE()				__HAL_RCC_GPIOB_CLK_ENABLE()
#define GPIOB_CLK_DISABLE()				__HAL_RCC_GPIOB_CLK_DISABLE()

void ADC_Config(void);
void ADC_Init();
void DMA_Config();
void DMA_Init();
void valorTension();


