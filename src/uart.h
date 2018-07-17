/*
 * uart.h
 *
 *  Created on: 24 abr. 2017
 *      Author: Larraitz
 */

#define GPIOA_CLK_ENABLE()				__HAL_RCC_GPIOA_CLK_ENABLE()
#define GPIOA_CLK_DISABLE()				__HAL_RCC_GPIOA_CLK_DISABLE()

#define GPIOC_CLK_ENABLE()				__HAL_RCC_GPIOC_CLK_ENABLE()
#define GPIOC_CLK_DISABLE()				__HAL_RCC_GPIOC_CLK_DISABLE()

#define USART6_CLK_ENABLE()				__USART6_CLK_ENABLE();
#define USART2_CLK_ENABLE()				__USART6_CLK_ENABLE();


void UART_Config(void);
void UART6_Init(void);
void UART2_Init(void);
void UART1_Init(void);
