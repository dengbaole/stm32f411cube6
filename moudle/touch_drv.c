#include "touch_drv.h"

uint8_t tp_buff[20] = {0x10, 0x12, 0x13};


void tp_gpio_init(void) {
	GPIO_InitTypeDef gpio_init_struct;
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	gpio_init_struct.Pin = TP_CS_PIN;
	gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(TP_CS_PORT, &gpio_init_struct);
	TP_CS_HIGH();

	gpio_init_struct.Pin = TP_IRQ_PIN;
	gpio_init_struct.Mode = GPIO_MODE_INPUT;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(TP_IRQ_PORT, &gpio_init_struct);


}
void tp_init(void) {
	tp_gpio_init();
	spi2_init();
	tp_buff[0] = 0xd0;
	tp_spi_send(tp_buff,1);
	HAL_Delay(100);
	tp_spi_receive(tp_buff, 2);
	tp_buff[3] = 0xd0;
}


void tp_spi_send(uint8_t* data, uint16_t size) {
	TP_CS_LOW();
	spi2_send(data, size);
	TP_CS_HIGH();
}


void tp_spi_receive(uint8_t* data, uint16_t size) {
	TP_CS_LOW();
	spi2_receive(data, size);
	TP_CS_HIGH();
}

