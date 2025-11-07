#ifndef _PLATFORM_H_
#define _PLATFORM_H_


#include "stm32f4xx.h"
#include "stm32f4xx_hal_conf.h"

#include "stdbool.h"
#include "uevent.h"
#include "even.h"
#include "bsp_timer.h"
#include "led_drv.h"
#include "spi_drv.h"
#include "tft_drv.h"

//事件定义
#define UEVT_APP_BASE (0xAE00)
// 上电信号
#define UEVT_APP_POWERUP (UEVT_APP_BASE | 0x01)
// Boot阶段
#define UEVT_APP_BOOT (UEVT_APP_BASE | 0x0B)
// 初始化段
#define UEVT_APP_SETUP (UEVT_APP_BASE | 0x02)
// 多任务开始
#define UEVT_APP_START (UEVT_APP_BASE | 0x03)
// 准备休眠信号
#define UEVT_APP_BEFORE_SLEEP (UEVT_APP_BASE | 0x0E)

// mark 按键事件
#define UEVT_RTC_BASE (0x0000)
#define UEVT_RTC_1MS (UEVT_RTC_BASE | 0x03)
#define UEVT_RTC_10MS (UEVT_RTC_BASE | 0x04)
#define UEVT_RTC_100MS (UEVT_RTC_BASE | 0x05)

#define UEVT_BTN_BASE (0x0100)
#define UEVT_BTN_DOWN_DOWN (UEVT_BTN_BASE | 0x01)
#define UEVT_BTN_DOWN_RELEASE (UEVT_BTN_BASE | 0x02)
#define UEVT_BTN_DOWN_LONG (UEVT_BTN_BASE | 0x03)
#define UEVT_BTN_DOWN_REPEAT (UEVT_BTN_BASE | 0x04)

#define UEVT_BTN_UP_DOWN (UEVT_BTN_BASE | 0x11)
#define UEVT_BTN_UP_RELEASE (UEVT_BTN_BASE | 0x12)
#define UEVT_BTN_UP_LONG (UEVT_BTN_BASE | 0x13)
#define UEVT_BTN_UP_REPEAT (UEVT_BTN_BASE | 0x14)


#define UEVT_BTN_RIGHT_DOWN (UEVT_BTN_BASE | 0x21)
#define UEVT_BTN_RIGHT_RELEASE (UEVT_BTN_BASE | 0x22)
#define UEVT_BTN_RIGHT_LONG_WORK (UEVT_BTN_BASE | 0x23)
#define UEVT_BTN_RIGHT_REPEAT (UEVT_BTN_BASE | 0x24)

#define UEVT_BTN_LEFT_DOWN (UEVT_BTN_BASE | 0x31)
#define UEVT_BTN_LEFT_RELEASE (UEVT_BTN_BASE | 0x32)
#define UEVT_BTN_LEFT_LONG_WORK (UEVT_BTN_BASE | 0x33)
#define UEVT_BTN_LEFT_REPEAT (UEVT_BTN_BASE | 0x34)

// 组合按键事件
#define UEVT_BTN_UPDOWN_DOWN (UEVT_BTN_BASE | 0x41)
#define UEVT_BTN_UPDOWN_LONG (UEVT_BTN_BASE | 0x43)
#define UEVT_BTN_UPMAIN_DOWN (UEVT_BTN_BASE | 0x45)
#define UEVT_BTN_DOWNMAIN_DOWN (UEVT_BTN_BASE | 0x46)


// 在触发工作之前松开为一次超短按事件
// 超短按事件发生后250ms内没有按键，则触发HIT-N事件
// 如果有按键，则触发HIT计数增加
#define UEVT_BTN_RIGHT_HIT0 (UEVT_BTN_BASE | 0x30)
#define UEVT_BTN_RIGHT_HIT1 (UEVT_BTN_RIGHT_HIT0 + 1)
#define UEVT_BTN_RIGHT_HIT2 (UEVT_BTN_RIGHT_HIT0 + 2)
#define UEVT_BTN_RIGHT_HIT3 (UEVT_BTN_RIGHT_HIT0 + 3)
#define UEVT_BTN_RIGHT_HIT4 (UEVT_BTN_RIGHT_HIT0 + 4)
#define UEVT_BTN_RIGHT_HIT5 (UEVT_BTN_RIGHT_HIT0 + 5)
#define UEVT_BTN_RIGHT_HIT6P (UEVT_BTN_RIGHT_HIT0 + 6)



extern uint8_t time_m;
extern uint8_t time_h;
extern uint8_t beep_on_times;




void platform_init(void);

#endif // !_PLATFORM_H_

