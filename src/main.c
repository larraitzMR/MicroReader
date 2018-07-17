/*
 * main.c
 *
 *  Created on: 3 abr. 2017
 *      Author: Larraitz
 */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "display.h"
#include "delay.h"
#include "ADC.h"
#include "bateria.h"
#include "uart.h"
#include "gps_v0.h"
#include "spi.h"
#include "timer.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static GPIO_InitTypeDef GPIO_InitStruct;
extern UART_HandleTypeDef huart2;
extern SPI_HandleTypeDef hspi2;
extern TIM_HandleTypeDef htim2;

#define BUFFERSIZE 6
#define RXBUFFSIZE 200

/* Buffer used for reception */
uint8_t aRxBuffer[BUFFERSIZE];
char RxBuf[RXBUFFSIZE];

char ReadyBuf[5];
extern int ready;
extern uint8_t ReadyMsg[];
char buff[8];


/**
 * @brief  Inicializacion del led de la placa
 * @param  None
 * @retval None
 */
void init_LED2() {

	LED2_GPIO_CLK_ENABLE();
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Pin = LED2_PIN;
	HAL_GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStruct);
}

/*
 * @brief  Main program
 * @param  None
 * @retval None
 */
int main(void) {

 	HAL_Init();
	SystemClock_Config();

//	delay_Init();
	TIM2_Config();
	TIM2_Init();

	LCD_Config();
	LCD_Init();
	LCD_Command(LCD_CLEAR_DISPLAY);

//	ADC_Config();
	DMA_Config();
//	ADC_Init();
	DMA_Init();
	crear_matrices();

	UART_Config();
	UART6_Init();
	UART2_Init();

	SPI_Config();
	SPI_Init();

	char *msg = "\n\rEsto se imprime por TeraTerm!\n\r";
	HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 0xFFFF);

	inicializar_gps();

	while (1) {

//		valorTension();
//		visualizar_coordenadas();

		if(ready == 0){
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
			HAL_SPI_TransmitReceive(&hspi2, (uint8_t*)ReadyMsg, (uint8_t *)ReadyBuf, 7, 5000);
			while (HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY) {
			}
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
			if(Buffercmp((uint8_t*)ReadyMsg, (uint8_t*)ReadyBuf, BUFFERSIZE))
			{
				HAL_UART_Transmit(&huart2, (uint8_t*) ReadyBuf, strlen(ReadyBuf), 1000);
				ready = 1;
			} else {
				HAL_UART_Transmit(&huart2, (uint8_t*) ReadyBuf, strlen(ReadyBuf), 1000);
			}
			Flush_Buffer((uint8_t*)ReadyBuf, BUFFERSIZE);
		}
//		enviar_coord_lora();
	}
}


/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSI)
 *            SYSCLK(Hz)                     = 84000000
 *            HCLK(Hz)                       = 84000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 2
 *            APB2 Prescaler                 = 1
 *            HSI Frequency(Hz)              = 16000000
 *            PLL_M                          = 16
 *            PLL_N                          = 336
 *            PLL_P                          = 4
 *            PLL_Q                          = 7
 *            VDD(V)                         = 3.3
 *            Main regulator output voltage  = Scale2 mode
 *            Flash Latency(WS)              = 2
 * @param  None
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;

	/* Enable Power Control clock */
	__HAL_RCC_PWR_CLK_ENABLE();

	/* The voltage scaling allows optimizing the power consumption when the device is
	 clocked below the maximum system frequency, to update the voltage scaling value
	 regarding system frequency refer to product datasheet.  */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

	/* Enable HSI Oscillator and activate PLL with HSI as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = 0x10;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 16;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
	 clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
void Error_Handler(void) {
	while (1) {
		char * linea = "Error!";
		HAL_UART_Transmit(&huart2, (uint8_t*)linea, sizeof(linea), 1000);
	}
}

