#pragma once

#include "globals.h"
#include "UART/uart.h"
#include "PWM/pwm_timer.h"
#include "GPIO/gpio.h"

#define MGT_UART USART1

// Coil addresses
#define MGT_0_GPIO  GPIOA

bool mgt_setPercentCoilAck(uint8_t coil_num, uint8_t pwm_num, uint8_t pwm_percentage);

bool mgt_getCoilCurrentAck(uint8_t coil_num);

bool mgt_ShutdownAck();

bool mgt_timerOffAck(const TIM_TypeDef* timer);

