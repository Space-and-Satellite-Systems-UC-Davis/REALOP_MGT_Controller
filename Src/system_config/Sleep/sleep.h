

#ifndef REALOP1_SLEEP_H
#define REALOP1_SLEEP_H

#include "stm32l431xx.h"
#include <globals.h>


#define USART1_WAKEUP 26
#define USART2_WAKEUP 27
#define USART3_WAKEUP 28
#define UART4_WAKEUP 29
#define LPUART1_WAKEUP 31
#define SLEEP 0
#define LPSLEEP 1
#define STOP0 2

// end testing stuff

void sleep_init();

void lowPowerSleep_init();

void stop0_init();






























#endif