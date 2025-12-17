#include "touch_xtp2046_drv.h"

uint8_t tp_tx_buff[3] = {0};
uint8_t tp_rx_buff[3] = {0};

void tp_gpio_init(void) {
	GPIO_InitTypeDef gpio_init_struct;
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	gpio_init_struct.Pin = TP_CS_PIN;
	gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(TP_CS_PORT, &gpio_init_struct);
	TP_CS_HIGH();

	gpio_init_struct.Pin = TP_IRQ_PIN;
	gpio_init_struct.Mode = GPIO_MODE_INPUT;
	gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(TP_IRQ_PORT, &gpio_init_struct);


}
void tp_init(void) {
	tp_gpio_init();
	spi2_init();
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

/**
 * @brief  读取 XTP2046 的 ADC 值
 * @param  cmd: 读取命令（X, Y, Z1, Z2）
 * @retval ADC 值（12位）
 */
// uint16_t tp_read_adc(uint8_t cmd) {
//     tp_tx_buff[0] = cmd;
// 	TP_CS_LOW();
// 	// for(volatile int i = 0; i < 10; i++);
// 	HAL_Delay(1);
// 	// 发送命令字节
// 	spi2_send(tp_tx_buff, 1);
// 	HAL_Delay(10);

// 	// 接收2个字节的ADC数据
// 	memset(tp_rx_buff,0,sizeof(tp_rx_buff));
// 	spi2_receive(tp_rx_buff, 2);

// 	TP_CS_HIGH();

// 	// 组合12位ADC值
// 	return ((tp_rx_buff[0] << 8) | tp_rx_buff[1]) >> 4;
// }

uint16_t tp_read_adc(uint8_t cmd) {
    uint8_t tx[3] = {0};
    uint8_t rx[3] = {0};
    tx[0] = cmd;

    TP_CS_LOW();
    
    // 使用全双工传输或顺序传输，去掉所有 HAL_Delay
    // 发送命令的同时接收第一个字节（通常是0）
    HAL_SPI_TransmitReceive(&hspi2, tx, rx, 3, 10); 
    
    TP_CS_HIGH();

    // XPT2046 返回的是 12 位数据，分布在 rx[1] 和 rx[2] 中
    // 具体的位移取决于芯片手册，通常是：
    uint16_t res = ((uint16_t)rx[1] << 8) | rx[2];
    return res >> 4; // 12位有效数据
}



/**
 * @brief  检测触摸是否按下
 * @retval 1: 按下, 0: 未按下
 */
uint8_t tp_is_pressed(void) {
	// XTP2046 的 IRQ 引脚在有触摸时输出低电平
	return (IS_TP_IRQ_ON() == GPIO_PIN_RESET);
}



/**
 * @brief  获取触摸坐标
 * @param  x: X坐标指针
 * @param  y: Y坐标指针
 */
void tp_get_xy(uint16_t* x, uint16_t* y) {
	uint16_t adc_x, adc_y;

	// 读取多次取平均值，提高精度
	adc_x = 0;
	adc_y = 0;

	for(uint8_t i = 0; i < 4; i++) {
		adc_x += tp_read_adc(TP_CMD_READ_X);
		adc_y += tp_read_adc(TP_CMD_READ_Y);
	}

	*x = adc_x >> 2;  // 除以4求平均
	*y = adc_y >> 2;
}




/**
 * @brief  获取触摸压力
 * @retval 压力值
 */
uint16_t tp_read_pressure(void) {
	uint16_t z1, z2;
	uint16_t pressure = 0;

	if(tp_is_pressed()) {
		z1 = tp_read_adc(TP_CMD_READ_Z1);
		z2 = tp_read_adc(TP_CMD_READ_Z2);

		if(z1 != 0) {
			// 计算压力（根据XPT2046手册公式）
			pressure = 4095 * tp_read_adc(TP_CMD_READ_X) / z1;
		}
	}

	return pressure;
}



void tp_calibrate_coords(uint16_t raw_x, uint16_t raw_y, uint16_t* screen_x, uint16_t* screen_y) {
	// 限制在有效范围内
	if(raw_x < tp_cal.x_min) {
		raw_x = tp_cal.x_min;
	}
	if(raw_x > tp_cal.x_max) {
		raw_x = tp_cal.x_max;
	}
	if(raw_y < tp_cal.y_min) {
	 	raw_y = tp_cal.y_min;
	}
	if(raw_y > tp_cal.y_max) {
	 	raw_y = tp_cal.y_max;
	}

	// 线性映射到屏幕坐标
	*screen_x = (tp_cal.x_max - (raw_x - tp_cal.x_min)) * tp_cal.width /
				(tp_cal.x_max - tp_cal.x_min);
	*screen_y = (raw_y - tp_cal.y_min) * tp_cal.height /
				(tp_cal.y_max - tp_cal.y_min);
}
                         