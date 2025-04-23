#ifndef REALOP1_GLOBALS_H_
#define REALOP1_GLOBALS_H_

#include "stm32l431xx.h"

#define UNUSED_GPIO             GPIOH, 1
#define LOCK_UNUSED_GPIO_HIGH() GPIOH->PUPDR |= (1 << GPIO_PUPDR_PUPD1)

typedef enum {false, true} bool;
#define NULL 0

#define WILL_LOOPS_TIMEOUT false
#define DEFAULT_TIMEOUT_MS 1000

void delay_ms(uint64_t ms);
void nop(long long nop_loops);
uint64_t getSysTime();

bool is_time_out(uint64_t start_time, uint64_t timeout_ms);
void while_timeout(void (*do_work)(), bool (*should_continue)(), uint64_t timeout_ms);
void wait_with_timeout(bool (*continue_waiting)(), uint64_t timeout_ms);
bool is_GPIOA_not_ready();
bool is_GPIOB_not_ready();
bool is_GPIOC_not_ready();

#endif // REALOP1_GLOBALS_H_
