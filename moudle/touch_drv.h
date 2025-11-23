#ifndef __TOUCH_DRV_H__
#define __TOUCH_DRV_H__
#include "platform.h"

extern uint8_t tp_buff[20];

#define TP_CLOCK_PIN    GPIO_PIN_13
#define TP_CLOCK_PORT   GPIOB

#define TP_MISO_PIN  GPIO_PIN_14
#define TP_MISO_PORT GPIOB

#define TP_MOSI_PIN  GPIO_PIN_15
#define TP_MOSI_PORT GPIOB

#define TP_CS_PIN    GPIO_PIN_8
#define TP_CS_PORT   GPIOA


#define TP_IRQ_PIN  GPIO_PIN_12
#define TP_IRQ_PORT GPIOB


#define TP_CS_LOW()  HAL_GPIO_WritePin(TP_CS_PORT, TP_CS_PIN, GPIO_PIN_RESET)
#define TP_CS_HIGH()  HAL_GPIO_WritePin(TP_CS_PORT, TP_CS_PIN, GPIO_PIN_SET)
#define IS_TP_IRQ_ON()   HAL_GPIO_ReadPin(TP_IRQ_PORT,TP_IRQ_PIN)  //0是按下


void tp_spi_send(uint8_t* data, uint16_t size);
void tp_spi_receive(uint8_t* data, uint16_t size);

#endif


