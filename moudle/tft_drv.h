
#ifndef _TFT_DRV_H
#define _TFT_DRV_H
#include "platform.h"

#define USE_HORIZONTAL 1


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
	#define LCD_W 240
	#define LCD_H 320

#else
	#define LCD_W 320
	#define LCD_H 240
#endif

#define LCD_DC_PIN  GPIO_PIN_14
#define DC_PORT GPIOB

#define LCD_RES_PIN  GPIO_PIN_12
#define RST_PORT GPIOB

#define CS_PIN    GPIO_PIN_15
#define CS_PORT   GPIOB

#define TFT_CS_LOW()  HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET)
#define TFT_CS_HIGH()  HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET)
#define TFT_RS_CMD()   HAL_GPIO_WritePin(DC_PORT,LCD_DC_PIN,GPIO_PIN_RESET)//DC
#define TFT_RS_DATA()   HAL_GPIO_WritePin(DC_PORT,LCD_DC_PIN,GPIO_PIN_SET)
#define LCD_RES_Clr()  HAL_GPIO_WritePin(RST_PORT,LCD_RES_PIN,GPIO_PIN_RESET)//RES
#define LCD_RES_Set()  HAL_GPIO_WritePin(RST_PORT,LCD_RES_PIN,GPIO_PIN_SET)

void lcd_init(void);
void LCD_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color);
void LCD_Clear(uint16_t Color);
#endif /*__ pinoutConfig_H */


