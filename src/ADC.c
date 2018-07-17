/*
 * DAC.c
 *
 *  Created on: 29 mar. 2017
 *      Author: Larraitz
 */

#include "main.h"
#include "ADC.h"
#include "display.h"
#include "bateria.h"

ADC_HandleTypeDef ADCHandle;
ADC_ChannelConfTypeDef ADCChannel;
DMA_HandleTypeDef Dma_Adc;

/**
  * @brief  Configuracion del ADC
  * @param  None
  * @retval None
  */
void ADC_Config(void) {

	GPIO_InitTypeDef GPIO_InitStruct;

	GPIOB_CLK_ENABLE();
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* ADC1 configuration ------------------------------------------------------*/
	HAL_ADC_DeInit(&ADCHandle);
	ADC1_CLK_ENABLE();

	ADCHandle.Instance = ADC1;

	/* Para ADC sin interrupcion*/
//	ADCHandle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;
//	ADCHandle.Init.Resolution = ADC_RESOLUTION_12B;
//	ADCHandle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
//	ADCHandle.Init.ScanConvMode = ENABLE;
//	ADCHandle.Init.EOCSelection = DISABLE; //ADC_EOC_SINGLE_CONV para inkesta/interrupcion
//	ADCHandle.Init.ContinuousConvMode = ENABLE;
//	ADCHandle.Init.NbrOfConversion = 1;
//	ADCHandle.Init.DiscontinuousConvMode = DISABLE;
//	ADCHandle.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
//	ADCHandle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
//	ADCHandle.Init.DMAContinuousRequests = ENABLE;

	/*Para ADC por interrupcion*/
	ADCHandle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;
	ADCHandle.Init.Resolution = ADC_RESOLUTION_12B;
	ADCHandle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	ADCHandle.Init.ScanConvMode = DISABLE;
	ADCHandle.Init.EOCSelection = ADC_EOC_SINGLE_CONV; //ADC_EOC_SINGLE_CONV para inkesta/interrupcion
	ADCHandle.Init.ContinuousConvMode = ENABLE;
	ADCHandle.Init.NbrOfConversion = 1;
	ADCHandle.Init.DiscontinuousConvMode = DISABLE;
	ADCHandle.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
	ADCHandle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	ADCHandle.Init.DMAContinuousRequests = ENABLE;

	HAL_ADC_Init(&ADCHandle);

	ADCChannel.Channel = ADC_CHANNEL;
	ADCChannel.Rank = 1;
	ADCChannel.SamplingTime = ADC_SAMPLETIME_84CYCLES;

	HAL_ADC_ConfigChannel(&ADCHandle, &ADCChannel);

	HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(ADC_IRQn);
}

/**
  * @brief  Inicializacion del ADC
  * @param  None
  * @retval None
  */
void ADC_Init() {
//	HAL_ADC_Start(&ADCHandle);
	HAL_ADC_Start_IT(&ADCHandle);
}

/**
  * @brief  Configuracion del DMA
  * @param  None
  * @retval None
  */
void DMA_Config(void) {

	__DMA2_CLK_ENABLE()	;

	Dma_Adc.Instance = DMA2_Stream0;
	Dma_Adc.Init.Channel = DMA_CHANNEL_0;
	Dma_Adc.Init.Direction = DMA_PERIPH_TO_MEMORY;
	Dma_Adc.Init.PeriphInc = DMA_PINC_DISABLE;
	Dma_Adc.Init.MemInc = DMA_MINC_ENABLE;
	Dma_Adc.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	Dma_Adc.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	Dma_Adc.Init.Mode = DMA_NORMAL;
	Dma_Adc.Init.Priority = DMA_PRIORITY_LOW;
	Dma_Adc.Init.FIFOMode = DMA_FIFOMODE_DISABLE;

	HAL_DMA_Init(&Dma_Adc);
	__HAL_LINKDMA(&ADCHandle, DMA_Handle, Dma_Adc);

}

/**
  * @brief  Inicializacion del ADC
  * @param  None
  * @retval None
  */
void DMA_Init(void) {

	/* DMA interrupt init */
	HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}

uint16_t valor;

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle)
   {
//		valor = HAL_ADC_GetValue(AdcHandle);
	if (__HAL_ADC_GET_FLAG(AdcHandle, ADC_FLAG_EOC)) {
//		valor = HAL_ADC_GetValue(AdcHandle);
	}
	HAL_ADC_Start_IT(AdcHandle);
   }


int columna, fila, matriz, p;
float porcfila, porcentaje;

char tension[] = ("Tension: ");

/**
  * @brief  Se visualizan los valores de la bateria
  * @param  None
  * @retval None
  */
void valorTension() {
	LCD_Cursor(2);
//	LCD_Print(tension);
//	intToString4(valor);
	buscarvalor(valor, &matriz, &columna, &fila);
	if (valor <= 0) {
		LCD_Command(0x80);
		LCD_Data(0);
		LCD_Command(0x81);
		LCD_Data(0);
		LCD_Command(0x82);
		LCD_Data(0);
		LCD_Command(0x83);
		LCD_Data(0);
		porcentaje = 0;
	} else {
		porcfila = (float) (fila*5.0) / 160.0;
		switch (matriz) {
		case 0:
			LCD_Command(0x80);
			LCD_Data(0);
			LCD_Command(0x81);
			LCD_Data(0);
			LCD_Command(0x82);
			LCD_Data(0);
			LCD_Command(0x83);
			LCD_Data(0);
			LCD_Command(0x80);
			if (columna == 0) {
				LCD_Data(1);
				porcentaje = porcfila;
			}
			if (columna == 1) {
				LCD_Data(2);
				porcentaje = 5 + porcfila;
			}
			if (columna == 2) {
				LCD_Data(3);
				porcentaje = 10 + porcfila;
			}
			if (columna == 3) {
				LCD_Data(4);
				porcentaje = 15 + porcfila;
			}
			if (columna == 4) {
				LCD_Data(5);
				porcentaje = 20 + porcfila;
			}
			break;
		case 1:
			LCD_Command(0x80);
			LCD_Data(0);
			LCD_Command(0x81);
			LCD_Data(0);
			LCD_Command(0x82);
			LCD_Data(0);
			LCD_Command(0x83);
			LCD_Data(0);
			LCD_Command(0x80);
			LCD_Data(5);
			LCD_Command(0x81);
			if (columna == 0) {
				LCD_Data(1);
				porcentaje = 25 + porcfila;
			}
			if (columna == 1) {
				LCD_Data(2);
				porcentaje = 30 + porcfila;
			}
			if (columna == 2) {
				LCD_Data(3);
				porcentaje = 35 + porcfila;
			}
			if (columna == 3) {
				LCD_Data(4);
				porcentaje = 40 + porcfila;
			}
			if (columna == 4) {
				LCD_Data(5);
				porcentaje = 45 + porcfila;
			}
			break;
		case 2:
			LCD_Command(0x80);
			LCD_Data(0);
			LCD_Command(0x81);
			LCD_Data(0);
			LCD_Command(0x82);
			LCD_Data(0);
			LCD_Command(0x83);
			LCD_Data(0);
			LCD_Command(0x80);
			LCD_Data(5);
			LCD_Command(0x81);
			LCD_Data(5);
			LCD_Command(0x82);
			if (columna == 0) {
				LCD_Data(1);
				porcentaje = 50 + porcfila;
			}
			if (columna == 1) {
				LCD_Data(2);
				porcentaje = 55 + porcfila;
			}
			if (columna == 2) {
				LCD_Data(3);
				porcentaje = 60 + porcfila;
			}
			if (columna == 3) {
				LCD_Data(4);
				porcentaje = 65 + porcfila;
			}
			if (columna == 4) {
				LCD_Data(5);
				porcentaje = 70 + porcfila;
			}
			break;
		case 3:
			LCD_Command(0x80);
			LCD_Data(0);
			LCD_Command(0x81);
			LCD_Data(0);
			LCD_Command(0x82);
			LCD_Data(0);
			LCD_Command(0x83);
			LCD_Data(0);
			LCD_Command(0x80);
			LCD_Data(5);
			LCD_Command(0x81);
			LCD_Data(5);
			LCD_Command(0x82);
			LCD_Data(5);
			LCD_Command(0x83);
			if (columna == 0) {
				LCD_Data(1);
				porcentaje = 75 + porcfila;
			}
			if (columna == 1) {
				LCD_Data(2);
				porcentaje = 80 + porcfila;
			}
			if (columna == 2) {
				LCD_Data(3);
				porcentaje = 85 + porcfila;
			}
			if (columna == 3) {
				LCD_Data(4);
				porcentaje = 90 + porcfila;
			}
			if (columna == 4) {
				LCD_Data(5);
				porcentaje = 95 + porcfila;
			}
			break;

		default:
			break;
		}
	}
//	LCD_Cursor(2);
//	LCD_Print(tension);
//	intToString4(valor);
	LCD_Command(0x85);
	p = (int) porcentaje;
	LCD_Print_Char(0x25);
	intToString3(p);
}
