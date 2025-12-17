#include "spi_drv.h"


DMA_HandleTypeDef hdma_spi1_tx;
DMA_HandleTypeDef hdma_spi1_rx;

GPIO_InitTypeDef GPIO_InitStruct;
SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

void spi1_init(void) {
    // 1. 开启时钟
    __HAL_RCC_SPI1_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_DMA2_CLK_ENABLE(); 

    // 2. GPIO 配置
    GPIO_InitStruct.Pin = TFT_CLOCK_PIN | TFT_MISO_PIN | TFT_MOSI_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // 3. SPI1 参数配置
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE; // 如果屏幕不支持 Mode 1，尝试改为 SPI_PHASE_2EDGE
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2; 
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    
    if (HAL_SPI_Init(&hspi1) != HAL_OK) {
        // Error_Handler();
    }

    // ==========================================
    // 4. DMA TX (发送) 配置 [你缺失的部分]
    // STM32F4 SPI1_TX 通常是 DMA2_Stream3_Channel3
    // ==========================================
    hdma_spi1_tx.Instance = DMA2_Stream3;
    hdma_spi1_tx.Init.Channel = DMA_CHANNEL_3;
    hdma_spi1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH; // 内存到外设
    hdma_spi1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_spi1_tx.Init.MemInc = DMA_MINC_ENABLE;         // 内存地址自增
    hdma_spi1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_spi1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_spi1_tx.Init.Mode = DMA_NORMAL;
    hdma_spi1_tx.Init.Priority = DMA_PRIORITY_HIGH;     // 发送优先级高一点
    hdma_spi1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    
    if (HAL_DMA_Init(&hdma_spi1_tx) != HAL_OK) {
        // Error_Handler();
    }
    // 链接 DMA 句柄到 SPI 句柄
    __HAL_LINKDMA(&hspi1, hdmatx, hdma_spi1_tx);


    // 5. DMA RX (接收) 配置 [你原来的代码]
    hdma_spi1_rx.Instance = DMA2_Stream2;
    hdma_spi1_rx.Init.Channel = DMA_CHANNEL_3;
    hdma_spi1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_spi1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_spi1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_spi1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_spi1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_spi1_rx.Init.Mode = DMA_NORMAL;
    hdma_spi1_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_spi1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    
    if (HAL_DMA_Init(&hdma_spi1_rx) != HAL_OK) {
        // Error_Handler();
    }
    __HAL_LINKDMA(&hspi1, hdmarx, hdma_spi1_rx);

    // 6. 中断优先级配置
    // 必须要开启 Stream3 的中断，否则无法进入回调函数
    HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
    
    HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
    
    // 开启 SPI 中断（HAL库有些版本处理回调需要 SPI 全局中断）
    HAL_NVIC_SetPriority(SPI1_IRQn, 0, 2);
    HAL_NVIC_EnableIRQ(SPI1_IRQn);
}

void SPI1_IRQHandler(void) {
    HAL_SPI_IRQHandler(&hspi1);
}
void spi_send(uint8_t* data, uint16_t size) {
	HAL_SPI_Transmit_DMA(&hspi1, data, size);
}

void spi_receive(uint8_t* data, uint16_t size) {
	HAL_SPI_Receive_DMA(&hspi1, data, size);
}


void DMA2_Stream3_IRQHandler(void) {
	HAL_DMA_IRQHandler(&hdma_spi1_tx);
}

void DMA2_Stream2_IRQHandler(void) {
	HAL_DMA_IRQHandler(&hdma_spi1_rx);
}




void spi2_init(void) {
	__HAL_RCC_SPI2_CLK_ENABLE();

	__HAL_RCC_GPIOB_CLK_ENABLE();
	/**SPI2 GPIO Configuration
	PB13     ------> SPI2_SCK
	PB14     ------> SPI2_MISO
	PB15     ------> SPI2_MOSI
	*/
	GPIO_InitStruct.Pin = TP_CLOCK_PIN | TP_MISO_PIN | TP_MOSI_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	/* USER CODE BEGIN SPI2_Init 0 */

	/* USER CODE END SPI2_Init 0 */

	/* USER CODE BEGIN SPI2_Init 1 */

	/* USER CODE END SPI2_Init 1 */
	hspi2.Instance = SPI2;
	hspi2.Init.Mode = SPI_MODE_MASTER;
	hspi2.Init.Direction = SPI_DIRECTION_2LINES;
	hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi2.Init.NSS = SPI_NSS_SOFT;
	hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
	hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi2.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&hspi2) != HAL_OK) {
		//Error_Handler();
	}
	/* USER CODE BEGIN SPI2_Init 2 */

	/* USER CODE END SPI2_Init 2 */

}



void spi2_send(uint8_t* data, uint16_t size) {
	HAL_SPI_Transmit(&hspi2, data, size, HAL_MAX_DELAY);
}

void spi2_receive(uint8_t* data, uint16_t size) {
	HAL_SPI_Receive(&hspi2, data, size, HAL_MAX_DELAY);
}




void spi2_transfer(uint8_t* tx_data, uint8_t* rx_data, uint16_t size) {
	HAL_SPI_TransmitReceive(&hspi2, tx_data, rx_data, size, HAL_MAX_DELAY);
}
