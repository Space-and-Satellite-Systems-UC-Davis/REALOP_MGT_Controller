
#ifndef REALOP1_PWM_TIMER_H_
#define REALOP1_PWM_TIMER_H_


#include "stm32l431xx.h"
#include <globals.h>


#define PWMTimerDRV0 				   TIM2
#define PWMTimerDRV1                   TIM2
#define PWMTimerDRV2                   TIM1

typedef enum PWM_Channels {
    DRV0_PWM0, DRV0_PWM1, DRV1_PWM0, DRV1_PWM1, DRV2_PWM0, DRV2_PWM1
} PWM_Channels;


#define PWM_TIMER_ON()  PWMTimer->CR1 |=  TIM_CR1_CEN;
#define PWM_TIMER_OFF() PWMTimer->CR1 &= ~TIM_CR1_CEN;

bool pwm_initTimer(PWM_Channels channel, uint32_t period);
void pwm_setDutyCycle(PWM_Channels channel,uint8_t percentage);

void pwm_timerOn(TIM_TypeDef* timer);
void pwm_timerOff(TIM_TypeDef* timer);

void pwm_enableChannel(PWM_Channels channel);
void pwm_disableChannel(PWM_Channels channel);
void pwm_init();



#endif