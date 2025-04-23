#pragma once

#include "globals.h"
#include "UART/uart.h"
#include "PWM/pwm_timer.h"
#include "GPIO/gpio.h"

#define MGT_USART USART1

#define MGT_SHUTDOWN_CMD 'D'
#define MGT_SET_PWM_PERCENT_CMD 'S'
#define MGT_GET_CURRENT_CMD 'C'
#define MGT_TIMER_OFF_CMD 'T'

// Coil addresses
#define MGT_0_GPIO  GPIOA

bool mgt_mcu_setPWMCoil(uint8_t coil_num, uint8_t pwm_num, uint8_t pwm_percentage);

int8_t mgt_getCoilCurrent(uint8_t coil_num);

bool mgt_mcu_getCoilCurrent(uint8_t coil_num);

bool mgt_mcu_shutdown();

bool mgt_mcu_timerOff(uint8_t timer_num);

bool mgt_receive_packet(uint8_t arg0[], uint8_t arg1[], uint8_t arg2[]);