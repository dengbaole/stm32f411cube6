#include "platform.h"


uint8_t time_s = 0;
uint8_t time_m = 0;
uint8_t time_h = 0;
uint8_t beep_on_times = 3;


void platform_init(void) {
	HAL_Init();                         /* 初始化HAL库 */
	SystemClock_Config();
	module_init();
	led_init();
	LED_ON();

}
