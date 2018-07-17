/*
 * spi.c
 *
 *  Created on: 28 abr. 2017
 *      Author: Larraitz
 */

#include "main.h"
#include "spi.h"

SPI_HandleTypeDef hspi2;
DMA_HandleTypeDef hdma_tx;
DMA_HandleTypeDef hdma_rx;

void SPI_Config()
{
	GPIO_InitTypeDef GPIO_InitStruct;

	__SPI2_CLK_ENABLE();
	__GPIOB_CLK_ENABLE();

	/**SPI2 GPIO Configuration
	 PB15     ------> SPI2_MOSI
	 PB14     ------> SPI2_MISO
	 PB13     ------> SPI2_SCK
	 PB12     ------> SPI2_NSS
	 */

	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;	//GPIO_NOPULL
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_14;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_15;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

//	GPIO_InitStruct.Pin  = GPIO_PIN_12;
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* Peripheral interrupt init*/
	HAL_NVIC_SetPriority(SPI2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(SPI2_IRQn);

//    /*##-3- Configure the DMA ##################################################*/
//    /* Configure the DMA handler for Transmission process */
//    hdma_tx.Instance                 = SPIx_TX_DMA_STREAM;
//    hdma_tx.Init.Channel             = SPIx_TX_DMA_CHANNEL;
//    hdma_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
//    hdma_tx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
//    hdma_tx.Init.MemBurst            = DMA_MBURST_INC4;
//    hdma_tx.Init.PeriphBurst         = DMA_PBURST_INC4;
//    hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
//    hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
//    hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
//    hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
//    hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
//    hdma_tx.Init.Mode                = DMA_NORMAL;
//    hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;
//
//    HAL_DMA_Init(&hdma_tx);
//
//    /* Associate the initialized DMA handle to the the SPI handle */
//    __HAL_LINKDMA(&hspi2, hdmatx, hdma_tx);
//
//    /* Configure the DMA handler for Transmission process */
//    hdma_rx.Instance                 = SPIx_RX_DMA_STREAM;
//
//    hdma_rx.Init.Channel             = SPIx_RX_DMA_CHANNEL;
//    hdma_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
//    hdma_rx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
//    hdma_rx.Init.MemBurst            = DMA_MBURST_INC4;
//    hdma_rx.Init.PeriphBurst         = DMA_PBURST_INC4;
//    hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
//    hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
//    hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
//    hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
//    hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
//    hdma_rx.Init.Mode                = DMA_NORMAL;
//    hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;
//
//    HAL_DMA_Init(&hdma_rx);
//
//    /* Associate the initialized DMA handle to the the SPI handle */
//    __HAL_LINKDMA(&hspi2, hdmarx, hdma_rx);
//
//    /*##-4- Configure the NVIC for DMA #########################################*/
//    /* NVIC configuration for DMA transfer complete interrupt (SPI1_TX) */
//    HAL_NVIC_SetPriority(SPIx_DMA_TX_IRQn, 1, 1);
//    HAL_NVIC_EnableIRQ(SPIx_DMA_TX_IRQn);
//
//    /* NVIC configuration for DMA transfer complete interrupt (SPI1_RX) */
//    HAL_NVIC_SetPriority(SPIx_DMA_RX_IRQn, 1, 0);
//    HAL_NVIC_EnableIRQ(SPIx_DMA_RX_IRQn);


}


void SPI_Init(void)
{
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;

  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
  hspi2.Init.CRCPolynomial = 7;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.NSS = SPI_NSS_SOFT; //SPI_NSS_HARD_OUTPUT
  hspi2.Init.TIMode = SPI_TIMODE_DISABLED;

  HAL_SPI_Init(&hspi2);
}

void SPI_DeInit(void)
{
    /*##-1- Reset peripherals ##################################################*/
//    SPIx_FORCE_RESET();
//    SPIx_RELEASE_RESET();

    /*##-2- Disable peripherals and GPIO Clocks ################################*/
    /* Deconfigure SPI SCK */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_13);
    /* Deconfigure SPI MISO */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_14);
    /* Deconfigure SPI MOSI */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_15);

    /*##-3- Disable the DMA ####################################################*/
    /* De-Initialize the DMA associated to transmission process */
    HAL_DMA_DeInit(&hdma_tx);
    /* De-Initialize the DMA associated to reception process */
    HAL_DMA_DeInit(&hdma_rx);

    /*##-4- Disable the NVIC for DMA ###########################################*/
    HAL_NVIC_DisableIRQ(SPIx_DMA_TX_IRQn);
    HAL_NVIC_DisableIRQ(SPIx_DMA_RX_IRQn);
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 0  : pBuffer1 identical to pBuffer2
  *         >0 : pBuffer1 differs from pBuffer2
  */
uint16_t Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if ((*pBuffer1) != *pBuffer2)
    {
      return BufferLength;
    }
    pBuffer1++;
    pBuffer2++;
  }

  return 0;
}

/**
  * @brief  Flushes the buffer
  * @param  pBuffer: buffers to be flushed.
  * @param  BufferLength: buffer's length
  * @retval None
  */
void Flush_Buffer(uint8_t* pBuffer, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    *pBuffer = 0;

    pBuffer++;
  }
}

//uint8_t aTxBuffer[] = "****SPI - Two Boards communication based on Interrupt **** SPI Message ******** SPI Message ******** SPI Message ****";
#define BUFFERSIZE 117
uint8_t aTxBuf[] = "****SPI - Two Boards communication based on Interrupt **** SPI Message ******** SPI Message ******** SPI Message ****";
//extern __IO ITStatus SPIReady;
extern UART_HandleTypeDef huart2;
extern __IO ITStatus UartReady;
/**
  * @brief  Tx Transfer completed callback
  * @param  hspi: SPI handle.
  * @retval None
  */
//void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
//{
////	SPIReady = SET;
////	HAL_SPI_Transmit_IT(&hspi2, (uint8_t*) aTxBuf, BUFFERSIZE);
//	char * linea = "Enviado!";
//	HAL_UART_Transmit(&huart2, (uint8_t*)linea, sizeof(linea), 1000);
//	while (UartReady != SET) {
//	}
//	UartReady = RESET;
//}


