/*
 * spi.h
 *
 *  Created on: 28 abr. 2017
 *      Author: Larraitz
 */

#define DMAx_CLK_ENABLE()                __HAL_RCC_DMA2_CLK_ENABLE()

/* Definition for SPIx's DMA */
#define SPIx_TX_DMA_STREAM               DMA2_Stream3
#define SPIx_RX_DMA_STREAM               DMA2_Stream2
#define SPIx_TX_DMA_CHANNEL              DMA_CHANNEL_3
#define SPIx_RX_DMA_CHANNEL              DMA_CHANNEL_3


/* Definition for SPIx's NVIC */
#define SPIx_DMA_TX_IRQn                 DMA2_Stream3_IRQn
#define SPIx_DMA_RX_IRQn                 DMA2_Stream2_IRQn

void SPI_Init();
void SPI_Config(void);
uint16_t Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2, uint16_t BufferLength);
void Flush_Buffer(uint8_t* pBuffer, uint16_t BufferLength);
