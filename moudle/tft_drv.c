#include "tft_drv.h"

/****************************************************************************************************
//=========================================电源接线================================================//
//     LCD模块                STM32单片机
//      VCC          接        DC5V/3.3V      //电源
//      GND          接          GND          //电源地
//=======================================液晶屏数据线接线==========================================//
//本模块默认数据总线类型为SPI总线
//     LCD模块                STM32单片机
//    SDI(MOSI)      接          PB5          //液晶屏SPI总线数据写信号
//    SDO(MISO)      接          PB4          //液晶屏SPI总线数据读信号，如果不需要读，可以不接线
//       SCK         接          PB3          //液晶屏SPI总线时钟信号
//=======================================液晶屏控制线接线==========================================//
//     LCD模块 					      STM32单片机
//       LED         接          PB13         //液晶屏背光控制信号，如果不需要控制，接5V或3.3V
//      DC/RS        接          PB14         //液晶屏数据/命令控制信号
//       RST         接          PB12         //液晶屏复位控制信号
//       CS          接          PB15         //液晶屏片选控制信号
//=========================================触摸屏触接线=========================================//
//如果模块不带触摸功能或者带有触摸功能，但是不需要触摸功能，则不需要进行触摸屏接线
//	   LCD模块                STM32单片机
//      T_IRQ        接          PB1          //触摸屏触摸中断信号
//      T_DO         接          PB2          //触摸屏SPI总线读信号
//      T_DIN        接          PF11         //触摸屏SPI总线写信号
//      T_CS         接          PC5          //触摸屏片选控制信号
//      T_CLK        接          PB0          //触摸屏SPI总线时钟信号
**************************************************************************************************/


void  tft_init(void) {
	GPIO_InitTypeDef gpio_init_struct;

	__HAL_RCC_GPIOB_CLK_ENABLE();

	gpio_init_struct.Pin = LCD_DC_PIN;
	gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(DC_PORT, &gpio_init_struct);
	HAL_GPIO_WritePin(LED_B_PORT, LED_B_PIN, GPIO_PIN_RESET);

	gpio_init_struct.Pin = LCD_RES_PIN;
	gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(RST_PORT, &gpio_init_struct);
	HAL_GPIO_WritePin(LED_B_PORT, LED_B_PIN, GPIO_PIN_RESET);

	gpio_init_struct.Pin = CS_PIN;
	gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(CS_PORT, &gpio_init_struct);
	HAL_GPIO_WritePin(LED_B_PORT, LED_B_PIN, GPIO_PIN_RESET);
}



void spi_send_data(uint8_t* data, uint16_t size) {
	// 使用HAL_SPI_Transmit函数发送数据
	if (HAL_SPI_Transmit(&hspi1, data, size, HAL_MAX_DELAY) != HAL_OK) {
		// 处理错误
		// 可以在这里添加错误处理代码
	}
}



void LCD_Writ_Bus(uint8_t dat) {
	TFT_CS_LOW();
	HAL_SPI_Transmit(&hspi1, &dat, 1, HAL_MAX_DELAY);
	TFT_CS_HIGH();
}



void LCD_WR_DATA8(uint8_t dat) {
	LCD_Writ_Bus(dat);
}



void LCD_WR_DATA(uint16_t dat) {
	LCD_Writ_Bus(dat >> 8);
	LCD_Writ_Bus(dat);
}



void LCD_WR_REG(uint8_t dat) {
	TFT_RS_CMD();
	LCD_Writ_Bus(dat);
	TFT_RS_DATA();
}


void lcd_init(void) {

	LCD_RES_Clr();
	HAL_Delay(10);
	LCD_RES_Set();
	HAL_Delay(10);
	HAL_Delay(10);
	LCD_WR_REG(0x11);     //Sleep out
	HAL_Delay(12);                //Delay 120ms
	LCD_WR_REG(0xB1);     //Normal mode
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x3C);
	LCD_WR_REG(0xB2);     //Idle mode
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x3C);
	LCD_WR_REG(0xB3);     //Partial mode
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x3C);
	LCD_WR_REG(0xB4);     //Dot inversion
	LCD_WR_DATA8(0x03);
	LCD_WR_REG(0xC0);     //AVDD GVDD
	LCD_WR_DATA8(0xAB);
	LCD_WR_DATA8(0x0B);
	LCD_WR_DATA8(0x04);
	LCD_WR_REG(0xC1);     //VGH VGL
	LCD_WR_DATA8(0xC5);   //C0
	LCD_WR_REG(0xC2);     //Normal Mode
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x00);
	LCD_WR_REG(0xC3);     //Idle
	LCD_WR_DATA8(0x8D);
	LCD_WR_DATA8(0x6A);
	LCD_WR_REG(0xC4);     //Partial+Full
	LCD_WR_DATA8(0x8D);
	LCD_WR_DATA8(0xEE);
	LCD_WR_REG(0xC5);     //VCOM
	LCD_WR_DATA8(0x0F);
	LCD_WR_REG(0xE0);     //positive gamma
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x0E);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x10);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x02);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x09);
	LCD_WR_DATA8(0x0F);
	LCD_WR_DATA8(0x25);
	LCD_WR_DATA8(0x36);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x10);
	LCD_WR_REG(0xE1);     //negative gamma
	LCD_WR_DATA8(0x0A);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x0F);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x02);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x09);
	LCD_WR_DATA8(0x0F);
	LCD_WR_DATA8(0x25);
	LCD_WR_DATA8(0x35);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x09);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x10);
	LCD_WR_REG(0xFC);
	LCD_WR_DATA8(0x80);
	LCD_WR_REG(0x3A);
	LCD_WR_DATA8(0x05);
	LCD_WR_REG(0x36);
	if(USE_HORIZONTAL == 0) {
		LCD_WR_DATA8(0x08);
	} else if(USE_HORIZONTAL == 1) {
		LCD_WR_DATA8(0xC8);
	} else if(USE_HORIZONTAL == 2)	{
		LCD_WR_DATA8(0x78);
	} else {
		LCD_WR_DATA8(0xA8);
	}
	LCD_WR_REG(0x21);     //Display inversion
	LCD_WR_REG(0x29);     //Display on

}