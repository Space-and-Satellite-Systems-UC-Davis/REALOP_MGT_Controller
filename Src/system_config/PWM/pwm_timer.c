/*
 * pwm_timers.c
 *
 *  - Nov 8-9, 2023
 *      Author       : nithinsenthil
 *      Log          : Updated pwm timer GPIO config for OP Rev2
 *
 *  - Unknown (Creation)
 *      Author       : Unknown
 *      Contributors : Unknown
 *      Log          : ESC controller functions
 */

#include "pwm_timer.h"
#include <globals.h>

// Global (external) variables and functions
extern int core_MHz;	// from core_config.h

/**
 *  Initializes the PWM pins
 */
void pwm_timer_gpio() {

	/* GPIO pinout
	 * 		TIM CH1		GPIO A15	AF - 1
	 * 		mgt-drv0-pwm1 	  PA0  	TIM2_CH1 AF1
	 *		mgt-drv0-pwm0	  PA1	TIM2_CH2 AF1
	 *		mgt-drv1-pwm1	  PB10	TIM2_CH3 AF1
	 *		mgt-drv1-pwm0	  PB11	TIM2_CH4 AF1
	 *		mgt-drv2-pwm0	  PA9	TIM1_CH2 AF1
	 *		mgt-drv2-pwm1	  PA8	TIM1_CH1 AF1
     *
	 */

	// Clock setup
	RCC->AHB2ENR  |= RCC_AHB2ENR_GPIOAEN;
	RCC->AHB2ENR  |= RCC_AHB2ENR_GPIOBEN;
	
	// Reset pin state
	GPIOA->MODER  &= ~(GPIO_MODER_MODE0_Msk |
					GPIO_MODER_MODE1_Msk | 
					GPIO_MODER_MODE8_Msk |
					GPIO_MODER_MODE9_Msk);
	GPIOB->MODER  &= ~(GPIO_MODER_MODE10_Msk | GPIO_MODER_MODE11_Msk);

	//Reset AF state
	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL0_Msk | GPIO_AFRL_AFSEL1_Msk);
	GPIOA->AFR[1] &= ~(GPIO_AFRH_AFSEL8_Msk | GPIO_AFRH_AFSEL9_Msk);
	GPIOB->AFR[1] &= ~(GPIO_AFRH_AFSEL10_Msk | GPIO_AFRH_AFSEL11_Msk);

	// Set pin mode
	GPIOA->MODER  |= (GPIO_MODER_MODE0_1 | GPIO_MODER_MODE1_1 | GPIO_MODER_MODE8_1 | GPIO_MODER_MODE9_1);
	GPIOB->MODER  |= (GPIO_MODER_MODE10_1 | GPIO_MODER_MODE11_1);

	// Set AF
	GPIOA->AFR[0] |= (1 << GPIO_AFRL_AFSEL0_Pos) | (1 << GPIO_AFRL_AFSEL1_Pos);
	GPIOA->AFR[1] |= (1 << GPIO_AFRH_AFSEL8_Pos) | (1 << GPIO_AFRH_AFSEL9_Pos);
	GPIOB->AFR[1] |= (1 << GPIO_AFRH_AFSEL10_Pos) | (1 << GPIO_AFRH_AFSEL11_Pos);

}

/**
 * Initializes PWM timers and GPIO, called inside platform init
 */
void pwm_init() {
	pwm_timer_gpio();
	pwm_timerClockEnable(PWMTimerDRV0);
	pwm_timerClockEnable(PWMTimerDRV1);
	pwm_timerClockEnable(PWMTimerDRV2);
}


bool pwm_initTimer(PWM_Channels channel, uint32_t period) {
	/* GPIO pinout
	 * 		TIM CH1		GPIO A15	AF - 1
	 *		mgt-drv0-pwm0	  PA1	TIM2_CH2 AF1
	 * 		mgt-drv0-pwm1 	  PA0  	TIM2_CH1 AF1

	 *		mgt-drv1-pwm0	  PB11	TIM2_CH4 AF1
	 *		mgt-drv1-pwm1	  PB10	TIM2_CH3 AF1

	 *		mgt-drv2-pwm0	  PA9	TIM1_CH2 AF1
	 *		mgt-drv2-pwm1	  PA8	TIM1_CH1 AF1
     *
	 */

	switch (channel) {
		case DRV0_PWM0:
		case DRV0_PWM1:
			PWMTimerDRV0->PSC = 2*(core_MHz/16) - 1;
			PWMTimerDRV0->ARR = period;
			PWMTimerDRV0->EGR |= TIM_EGR_UG;
			PWMTimerDRV0->CCMR1 = (6 << TIM_CCMR1_OC1M_Pos);
			PWMTimerDRV0->CCMR1 = (6 << TIM_CCMR1_OC2M_Pos);
			pwm_timerOn(PWMTimerDRV0);
			break;
		case DRV1_PWM0:
		case DRV1_PWM1:
			PWMTimerDRV1->PSC = 2*(core_MHz/16) - 1;
			PWMTimerDRV1->ARR = period;
			PWMTimerDRV1->EGR |= TIM_EGR_UG;
			PWMTimerDRV1->CCMR2 = (6 << TIM_CCMR2_OC3M_Pos);
			PWMTimerDRV1->CCMR2 = (6 << TIM_CCMR2_OC4M_Pos);
			pwm_timerOn(PWMTimerDRV1);
			break;
		case DRV2_PWM0:
		case DRV2_PWM1:
			PWMTimerDRV2->PSC = 2*(core_MHz/16) - 1;
			PWMTimerDRV2->ARR = period;
			PWMTimerDRV2->EGR |= TIM_EGR_UG;
			PWMTimerDRV2->CCMR1 = (6 << TIM_CCMR1_OC1M_Pos);
			PWMTimerDRV2->CCMR1 = (6 << TIM_CCMR1_OC2M_Pos);
			pwm_timerOn(PWMTimerDRV2);
			break;

	}

	return true;
}

void pwm_setDutyCycle(PWM_Channels channel, uint8_t percentage) {
	uint32_t period = 0;

	switch (channel) {
		case DRV0_PWM0:
			period = PWMTimerDRV0->ARR;
			PWMTimerDRV0->CCR2 = (int) (percentage * (period/100));
			break;
		case DRV0_PWM1:
			period = PWMTimerDRV0->ARR;
			PWMTimerDRV0->CCR1 = (int) (percentage * (period/100));
			break;
		case DRV1_PWM0:
			period = PWMTimerDRV1->ARR;
			PWMTimerDRV1->CCR4 = (int) (percentage * (period/100));
			break;
		case DRV1_PWM1:
			period = PWMTimerDRV1->ARR;
			PWMTimerDRV1->CCR3 = (int) (percentage * (period/100));
			break;
		case DRV2_PWM0:
			period = PWMTimerDRV2->ARR;
			PWMTimerDRV2->CCR2 = (int) (percentage * (period/100));
			break;
		case DRV2_PWM1:
			period = PWMTimerDRV2->ARR;
			PWMTimerDRV2->CCR1 = (int) (percentage * (period/100));
			break;
	}
}

void pwm_timerOn(TIM_TypeDef* timer) {
	timer->CR1 |= TIM_CR1_CEN;
}

void pwm_timerOff(TIM_TypeDef* timer) {
	timer->CR1 &= ~TIM_CR1_CEN;
}

void pwm_enableChannel(PWM_Channels channel) {
	switch (channel) {
		case DRV0_PWM0:
			PWMTimerDRV0->CCER |= TIM_CCER_CC2E;
			break;
		case DRV0_PWM1:
			PWMTimerDRV0->CCER |= TIM_CCER_CC1E;
			break;
		case DRV1_PWM0:
			PWMTimerDRV1->CCER |= TIM_CCER_CC4E;
			break;
		case DRV1_PWM1:
			PWMTimerDRV1->CCER |= TIM_CCER_CC3E;
			break;
		case DRV2_PWM0:
			PWMTimerDRV1->CCER |= TIM_CCER_CC2E;
			break;
		case DRV2_PWM1:
			PWMTimerDRV1->CCER |= TIM_CCER_CC1E;
			break;
	}
}


void pwm_disableChannel(PWM_Channels channel) {
	switch (channel) {
		case DRV0_PWM0:
			PWMTimerDRV0->CCER &= ~TIM_CCER_CC2E;
			break;
		case DRV0_PWM1:
			PWMTimerDRV0->CCER &= ~TIM_CCER_CC1E;
			break;
		case DRV1_PWM0:
			PWMTimerDRV1->CCER &= ~TIM_CCER_CC4E;
			break;
		case DRV1_PWM1:
			PWMTimerDRV1->CCER &= ~TIM_CCER_CC3E;
			break;
		case DRV2_PWM0:
			PWMTimerDRV1->CCER &= ~TIM_CCER_CC2E;
			break;
		case DRV2_PWM1:
			PWMTimerDRV1->CCER &= ~TIM_CCER_CC1E;
			break;
	}
}

void pwm_timerClockEnable(TIM_TypeDef *timer) {
	if (timer == PWMTimerDRV0 || timer == PWMTimerDRV1) {
		RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
	} else {
		RCC->APB1ENR1 |= RCC_APB2ENR_TIM1EN;
	}
}

void pwm_timerClockDisable(TIM_TypeDef *timer) {
	if (timer == PWMTimerDRV0 ||  timer == PWMTimerDRV1) {
		RCC->APB1ENR1 &= ~RCC_APB1ENR1_TIM2EN;
	} else {
		RCC->APB1ENR1 &= ~RCC_APB2ENR_TIM1EN;
	}
}
