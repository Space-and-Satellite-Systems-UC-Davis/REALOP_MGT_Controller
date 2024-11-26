#ifndef REALOP1_GLOBALS_H_
#define REALOP1_GLOBALS_H_

#include "stm32l431xx.h"

#define UNUSED_GPIO             GPIOH, 1
#define LOCK_UNUSED_GPIO_HIGH() GPIOH->PUPDR |= (1 << GPIO_PUPDR_PUPD1)

typedef enum {false, true} bool;
#define NULL 0


void delay_ms(uint64_t ms);
void nop(long long nop_loops);
uint64_t getSysTime();

#endif // REALOP1_GLOBALS_H_
