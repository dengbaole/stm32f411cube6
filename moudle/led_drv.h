#ifndef __LED_DRV_H
#define __LED_DRV_H

#include "platform.h"

#define LED_B_PIN           GPIO_PIN_13
#define LED_B_PORT           GPIOC

#define LED_ON()           HAL_GPIO_WritePin(LED_B_PORT, LED_B_PIN, GPIO_PIN_RESET);
#define LED_OFF()           HAL_GPIO_WritePin(LED_B_PORT, LED_B_PIN, GPIO_PIN_SET);

void led_init(void);

#endif

