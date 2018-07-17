/*
 * uart.c
 *
 *  Created on: 24 abr. 2017
 *      Author: Larraitz
 */

#include "uart.h"
#include "main.h"
#include "gps_v0.h"

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart6;
UART_HandleTypeDef huart2;
DMA_HandleTypeDef Dma_Uart;
DMA_HandleTypeDef Dma_Uart_rx;
DMA_HandleTypeDef Dma_Uart_tx;

/**
  * @brief  Configuracion de la UART
  * @param  None
  * @retval None
  */
void UART_Config(void)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIOC_CLK_ENABLE();

    /* Peripheral clock enable */
//    __USART1_CLK_ENABLE();
    __USART2_CLK_ENABLE();
    __USART6_CLK_ENABLE();

    /**USART1 GPIO Configuration
    PA10     ------> USART1_RX
    PB6     ------> USART1_TX
    */
//    GPIO_InitStruct.Pin = GPIO_PIN_10;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
//    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
//    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//
//    GPIO_InitStruct.Pin = GPIO_PIN_6 ;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
//    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
//    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


    /**USART2 GPIO Configuration
    PA2     ------> USART6_TX
    PA3     ------> USART6_RX
    */
    GPIO_InitStruct.Pin 	  	= GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode   		= GPIO_MODE_AF_PP; //GPIO_MODE_ANALOG?
    GPIO_InitStruct.Pull   		= GPIO_NOPULL;
    GPIO_InitStruct.Speed 	  	= GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate 	= GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /**USART6 GPIO Configuration
    PC6     ------> USART6_TX
    PC7     ------> USART6_RX
    PC8     ------> USART6_CK
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;//?? PULL_UP?
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* Peripheral DMA init*/

    Dma_Uart_rx.Instance = DMA2_Stream1;
    Dma_Uart_rx.Init.Channel = DMA_CHANNEL_5;
    Dma_Uart_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    Dma_Uart_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    Dma_Uart_rx.Init.MemInc = DMA_MINC_ENABLE;
    Dma_Uart_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    Dma_Uart_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    Dma_Uart_rx.Init.Mode = DMA_CIRCULAR;
    Dma_Uart_rx.Init.Priority = DMA_PRIORITY_LOW;
    Dma_Uart_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    HAL_DMA_Init(&Dma_Uart_rx);

    __HAL_LINKDMA(&huart6,hdmarx,Dma_Uart_rx);

    Dma_Uart_tx.Instance = DMA2_Stream6;
    Dma_Uart_tx.Init.Channel = DMA_CHANNEL_5;
    Dma_Uart_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    Dma_Uart_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    Dma_Uart_tx.Init.MemInc = DMA_MINC_ENABLE;
    Dma_Uart_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    Dma_Uart_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    Dma_Uart_tx.Init.Mode = DMA_CIRCULAR;
    Dma_Uart_tx.Init.Priority = DMA_PRIORITY_LOW;
    Dma_Uart_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    HAL_DMA_Init(&Dma_Uart_tx);

    __HAL_LINKDMA(&huart6,hdmatx,Dma_Uart_tx);

//    Dma_Uart.Instance = DMA2_Stream1;
//    Dma_Uart.Init.Channel = DMA_CHANNEL_5;
//    Dma_Uart.Init.Direction = DMA_PERIPH_TO_MEMORY;
//    Dma_Uart.Init.PeriphInc = DMA_PINC_DISABLE;
//    Dma_Uart.Init.MemInc = DMA_MINC_ENABLE;
//    Dma_Uart.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
//    Dma_Uart.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
//    Dma_Uart.Init.Mode = DMA_NORMAL;
//    Dma_Uart.Init.Priority = DMA_PRIORITY_LOW;
//    Dma_Uart.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
//    HAL_DMA_Init(&Dma_Uart);
//
//    __HAL_LINKDMA(&huart6,hdmarx,Dma_Uart);

    /*UART por interrupcion*/
//    HAL_NVIC_SetPriority(USART6_IRQn, 0, 0);
//    HAL_NVIC_EnableIRQ(USART6_IRQn);

    /* RX DMA*/
    HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
    /* TX DMA*/
    HAL_NVIC_SetPriority(DMA2_Stream6_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream6_IRQn);
}


/**
  * @brief  Inicializacion de la UART
  * @param  None
  * @retval None
  */
void UART1_Init(void) {

	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&huart1);

}

/**
  * @brief  Inicializacion de la UART
  * @param  None
  * @retval None
  */
void UART2_Init(void)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart2);

  HAL_NVIC_SetPriority(USART2_IRQn, 0x1, 0);
  HAL_NVIC_EnableIRQ(USART2_IRQn);
}

/**
  * @brief  Inicializacion de la UART
  * @param  None
  * @retval None
  */
void UART6_Init(void)
{
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 9600;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart6);

}


extern char reciboGPS[];
extern __IO ITStatus UartReady;

/**
  * @brief  Tx Transfer completed callback
  * @param  UartHandle: UART handle.
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
 {
	if (UartHandle->Instance == USART2) {
		HAL_NVIC_SetPendingIRQ(USART2_IRQn);
	} else if (UartHandle->Instance == USART6) {
		/* Set transmission flag: trasfer complete*/
		UartReady = SET;
		(&huart6)->gState = HAL_UART_STATE_READY;
	}
}


/**
  * @brief  Rx Transfer completed callback
  * @param  UartHandle: UART handle
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	if (UartHandle->Instance == USART2) {
		HAL_NVIC_SetPendingIRQ(USART2_IRQn);
	} else if (UartHandle->Instance == USART6) {
		/* Set transmission flag: trasfer complete*/
		UartReady = SET;
		guardar_coordenadas(reciboGPS);
//		HAL_UART_Transmit(&huart2, (uint8_t*) reciboGPS, strlen(reciboGPS),1000);
		(&huart6)->RxState = HAL_UART_STATE_READY;
	}
}

