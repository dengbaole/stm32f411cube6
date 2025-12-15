#ifndef __TOUCH_XTP2046_DRV_H__
#define __TOUCH_XTP2046_DRV_H__
#include "platform.h"

extern uint8_t tp_tx_buff[3];

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
// 触摸命令定义
#define TP_CMD_READ_X  0xD0
#define TP_CMD_READ_Y  0x90
#define TP_CMD_READ_Z1 0xB0
#define TP_CMD_READ_Z2 0xC0


// 校准数据结构
typedef struct {
	uint16_t x_min;
	uint16_t x_max;
	uint16_t y_min;
	uint16_t y_max;
	uint16_t width;   // 屏幕宽度
	uint16_t height;  // 屏幕高
	uint16_t tp_x;   
	uint16_t tp_y;  
	uint16_t tp_x_temp;   
	uint16_t tp_y_temp;  
} tp_calibration_t;

static tp_calibration_t tp_cal = {
	.x_min = 104,   // 根据实际校准调整
	.x_max = 1900,
	.y_min = 130,
	.y_max = 2000,
	.width = 240,   // 屏幕分辨率
	.height = 320,
    .tp_x = 0,
    .tp_y = 0,
    .tp_x_temp = 0,
    .tp_y_temp = 0
};



#define TP_CS_LOW()  HAL_GPIO_WritePin(TP_CS_PORT, TP_CS_PIN, GPIO_PIN_RESET)
#define TP_CS_HIGH()  HAL_GPIO_WritePin(TP_CS_PORT, TP_CS_PIN, GPIO_PIN_SET)
#define IS_TP_IRQ_ON()   HAL_GPIO_ReadPin(TP_IRQ_PORT,TP_IRQ_PIN)  //0是按下


void tp_gpio_init(void);
void tp_init(void);
void tp_spi_send(uint8_t* data, uint16_t size);
void tp_spi_receive(uint8_t* data, uint16_t size);
uint16_t tp_read_adc(uint8_t cmd);
uint8_t tp_is_pressed(void);
void tp_get_xy(uint16_t* x, uint16_t* y);
void tp_calibrate_coords(uint16_t raw_x, uint16_t raw_y,
						 uint16_t* screen_x, uint16_t* screen_y);


#endif


