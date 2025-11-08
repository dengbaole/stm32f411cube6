#include "bsp_timer.h"
#include "stm32f4xx.h"  // 包含 STM32F4 相关的定义

TIM_HandleTypeDef g_timx_handle;

/* 定时器中断初始化函数 */
void btim_timx_int_init(uint16_t arr, uint16_t psc) {
	__HAL_RCC_TIM1_CLK_ENABLE();  // 启用 TIM1 的时钟
	g_timx_handle.Instance = TIM1; // 使用 TIM1 实例
	g_timx_handle.Init.Prescaler = psc;
	g_timx_handle.Init.Period = arr;
	HAL_TIM_Base_Init(&g_timx_handle);
	HAL_TIM_Base_Start_IT(&g_timx_handle);
	HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 1, 3); // 设置 TIM1 的中断优先级
	HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn); // 启用 TIM1 的中断
}

/* 定时器1中断服务函数 */
void TIM1_UP_TIM10_IRQHandler(void) {
	HAL_TIM_IRQHandler(&g_timx_handle);
}

// #include "lvgl.h"
// #include "lv_port_disp.h"
// #include "lv_port_indev.h"
/* 定时器溢出中断回调函数 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
	static uint16_t counter;
	if (htim->Instance == TIM1) { // 判断实例是否为 TIM1
		// LED0_TOGGLE();
		// lv_tick_inc(1);
		uevt_bc_e(UEVT_RTC_1MS);  // 处理 1ms 的事件
		counter++;
		if ((counter % 10) == 0) {
			uevt_bc_e(UEVT_RTC_10MS);  // 处理 10ms 的事件
		}
		if ((counter % 100) == 0) {
			uevt_bc_e(UEVT_RTC_100MS);  // 处理 100ms 的事件
		}
	}
}
