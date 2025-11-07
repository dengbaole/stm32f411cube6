#include "led_drv.h"


void led_init(void) {
	GPIO_InitTypeDef gpio_init_struct;

	__HAL_RCC_GPIOC_CLK_ENABLE();

	gpio_init_struct.Pin = LED_B_PIN;
	gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LED_B_PORT, &gpio_init_struct);
}


