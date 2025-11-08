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
	TFT_RS_DATA();

	gpio_init_struct.Pin = LCD_RES_PIN;
	gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(RST_PORT, &gpio_init_struct);
	LCD_RES_Clr();

	gpio_init_struct.Pin = CS_PIN;
	gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(CS_PORT, &gpio_init_struct);
	TFT_CS_HIGH();
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

void Lcd_WriteData_16Bit(uint16_t Data) {
	LCD_Writ_Bus(Data >> 8);
	LCD_Writ_Bus(Data);
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


void LCD_direction(uint8_t direction) {
	switch(direction) {
		case 0:
			LCD_WR_REG(0x36);
			LCD_WR_REG((1 << 3) | (0 << 6) | (0 << 7)); //BGR==1,MY==0,MX==0,MV==0
			break;
		case 1:
			LCD_WR_REG(0x36);
			LCD_WR_REG((1 << 3) | (0 << 7) | (1 << 6) | (1 << 5));
			break;
		case 2:
			LCD_WR_REG(0x36);
			LCD_WR_REG((1 << 3) | (1 << 6) | (1 << 7));
			break;
		case 3:
			LCD_WR_REG(0x36);
			LCD_WR_REG((1 << 3) | (1 << 7) | (1 << 5));
			break;
		default:
			break;
	}
}

void lcd_init(void) {
	tft_init();
	spi_init();
	LCD_RES_Clr();
	HAL_Delay(100);
	LCD_RES_Set();
	HAL_Delay(50);
	// HAL_Delay(10);
	// LCD_WR_REG(0x11);     //Sleep out
	// HAL_Delay(12);                //Delay 120ms
	LCD_WR_REG(0xCF);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0xD9); //C1
	LCD_WR_DATA8(0X30);
	LCD_WR_REG(0xED);
	LCD_WR_DATA8(0x64);
	LCD_WR_DATA8(0x03);
	LCD_WR_DATA8(0X12);
	LCD_WR_DATA8(0X81);
	LCD_WR_REG(0xE8);
	LCD_WR_DATA8(0x85);
	LCD_WR_DATA8(0x10);
	LCD_WR_DATA8(0x7A);
	LCD_WR_REG(0xCB);
	LCD_WR_DATA8(0x39);
	LCD_WR_DATA8(0x2C);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x34);
	LCD_WR_DATA8(0x02);
	LCD_WR_REG(0xF7);
	LCD_WR_DATA8(0x20);
	LCD_WR_REG(0xEA);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);
	LCD_WR_REG(0xC0);    //Power control
	LCD_WR_DATA8(0x1B);   //VRH[5:0]
	LCD_WR_REG(0xC1);    //Power control
	LCD_WR_DATA8(0x12);   //SAP[2:0];BT[3:0] //0x01
	LCD_WR_REG(0xC5);    //VCM control
	LCD_WR_DATA8(0x26); 	 //3F
	LCD_WR_DATA8(0x26); 	 //3C
	LCD_WR_REG(0xC7);    //VCM control2
	LCD_WR_DATA8(0XB0);
	
	LCD_WR_REG(0x36);    // Memory Access Control
	LCD_WR_DATA8(0x08);
	// LCD_WR_DATA8(0x28);

	LCD_WR_REG(0x3A);
	LCD_WR_DATA8(0x55);
	LCD_WR_REG(0xB1);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x1A);
	LCD_WR_REG(0xB6);    // Display Function Control
	LCD_WR_DATA8(0x0A);
	LCD_WR_DATA8(0xA2);
	LCD_WR_REG(0xF2);    // 3Gamma Function Disable
	LCD_WR_DATA8(0x00);
	LCD_WR_REG(0x26);    //Gamma curve selected
	LCD_WR_DATA8(0x01);
	LCD_WR_REG(0xE0); //Set Gamma
	LCD_WR_DATA8(0x1F);
	LCD_WR_DATA8(0x24);
	LCD_WR_DATA8(0x24);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x12);
	LCD_WR_DATA8(0x09);
	LCD_WR_DATA8(0x52);
	LCD_WR_DATA8(0xB7);
	LCD_WR_DATA8(0x3F);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x15);
	LCD_WR_DATA8(0x06);
	LCD_WR_DATA8(0x0E);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x00);
	LCD_WR_REG(0XE1); //Set Gamma
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x1B);
	LCD_WR_DATA8(0x1B);
	LCD_WR_DATA8(0x02);
	LCD_WR_DATA8(0x0E);
	LCD_WR_DATA8(0x06);
	LCD_WR_DATA8(0x2E);
	LCD_WR_DATA8(0x48);
	LCD_WR_DATA8(0x3F);
	LCD_WR_DATA8(0x03);
	LCD_WR_DATA8(0x0A);
	LCD_WR_DATA8(0x09);
	LCD_WR_DATA8(0x31);
	LCD_WR_DATA8(0x37);
	LCD_WR_DATA8(0x1F);

	LCD_WR_REG(0x2B);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x01);
	LCD_WR_DATA8(0x3f);
	LCD_WR_REG(0x2A);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0xef);

	// LCD_WR_REG(0x36);
	// LCD_WR_REG(0xa0);

	LCD_WR_REG(0x11); //Exit Sleep
	HAL_Delay(120);
	LCD_WR_REG(0x29); //display on

	// LCD_direction(3);//设置LCD显示方向

}


void LCD_SetWindows(uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd) {
	LCD_WR_REG(0x2A);
	LCD_WR_DATA8(xStar >> 8);
	LCD_WR_DATA8(0x00FF & xStar);
	LCD_WR_DATA8(xEnd >> 8);
	LCD_WR_DATA8(0x00FF & xEnd);

	LCD_WR_REG(0x2B);
	LCD_WR_DATA8(yStar >> 8);
	LCD_WR_DATA8(0x00FF & yStar);
	LCD_WR_DATA8(yEnd >> 8);
	LCD_WR_DATA8(0x00FF & yEnd);

	LCD_WR_REG(0x2C);		//开始写入GRAM
}


void LCD_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t color) {
	uint16_t i, j;
	uint16_t width = ex - sx + 1; 		//得到填充的宽度
	uint16_t height = ey - sy + 1;		//高度
	LCD_SetWindows(sx, sy, ex, ey); //设置显示窗口
	for(i = 0; i < height; i++) {
		for(j = 0; j < width; j++) {
			Lcd_WriteData_16Bit(color);	//写入数据
		}
	}
}

void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos) {
	LCD_SetWindows(Xpos, Ypos, Xpos, Ypos);
}


void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color) {
	LCD_SetCursor(x, y); //设置光标位置
	Lcd_WriteData_16Bit(color);
}

void LCD_Clear(uint16_t Color) {
	uint16_t i, m;
	LCD_SetWindows(0, 0, 240 - 1, 320 - 1);
	TFT_CS_LOW();
	TFT_RS_DATA();
	for(i = 0; i < 240; i++) {
		for(m = 0; m < 320; m++) {
			Lcd_WriteData_16Bit(Color);
		}
	}
	TFT_CS_HIGH();
}