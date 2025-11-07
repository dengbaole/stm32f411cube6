#include "platform.h"


uint8_t time_s = 0;
uint8_t time_m = 0;
uint8_t time_h = 0;
uint8_t beep_on_times = 3;


void platform_init(void) {
	HAL_Init();                         /* 初始化HAL库 */
	SystemClock_Config();
	module_init();
	btim_timx_int_init(1000 - 1, 100);
	led_init();
	LED_ON();
	lcd_init();
	LCD_Clear(2323);
	LCD_Fill(0, 0, 240, 320, 1111);
}
