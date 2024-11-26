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

void pwm_timer_gpio() {

	/* OP R2 GPIO pinout
	 * 		TIM CH1		GPIO A15	AF - 1
	 */

	// Clock setup
	RCC->AHB2ENR  |= RCC_AHB2ENR_GPIOAEN;

	// Reset pin state
	GPIOA->MODER  &= ~GPIO_MODER_MODE15_Msk;
	GPIOA->AFR[1] &= ~GPIO_AFRH_AFSEL15_Msk;

	// Set pin mode
	GPIOA->MODER  |= (2U << GPIO_MODER_MODE15_Pos);

	// Set AF
	GPIOA->AFR[1] |= (1U << GPIO_AFRH_AFSEL15_Pos);

}


bool pwm_initTimer(uint32_t period) {
	// pwm_timer_gpio();
	// PWMTimer_ClockEnable();

	// (f_clk / f_goal) - 1  ==> ( 2*(core_MHz/16) / 1 MHz) - 1
	// (2*(core_MHz/16)) due to RCC configurations in core_config.c
 	// PWMTimer->PSC = 2*(core_MHz/16) - 1;		// Frequency in 1us;
	// PWMTimer->ARR = period;

	// PWMTimer->EGR |= TIM_EGR_UG;
	// PWMTimer->CCMR1 = (6 << TIM_CCMR1_OC1M_Pos);
	// PWMTimer->CCER |= TIM_CCER_CC1E;

	return true;
}

void pwm_setDutyCycle(PWM_Channels channel, uint8_t percentage) {
	switch (channel) {
		case DRV0_PWM0:
		case DRV0_PWM1:
			uint32_t period = PWMTimerDRV0->ARR;
			PWMTimerDRV0->CCR1 = (int) (percentage * (period/100));
			break;
		case DRV1_PWM0:
		case DRV1_PWM1:
			uint32_t period = PWMTimerDRV1->ARR;
			PWMTimerDRV1->CCR1 = (int) (percentage * (period/100));
			break;
		case DRV2_PWM0:
		case DRV2_PWM1:
			uint32_t period = PWMTimerDRV2->ARR;
			PWMTimerDRV2->CCR1 = (int) (percentage * (period/100));
			break;

	}
}

void pwm_timerClockEnable(TIM_TypeDef* timer) {
	if (timer == PWMTimerDRV0) {
		RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
	} else if(timer == PWMTimerDRV1) {
		RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
	} else {
		RCC->APB1ENR1 |= RCC_APB2ENR_TIM1EN;
	}
}

void pwm_timerClockDisable(TIM_TypeDef* timer) {
	if (timer == PWMTimerDRV0) {
		RCC->APB1ENR1 &= ~RCC_APB1ENR1_TIM2EN;
	} else if(timer == PWMTimerDRV1) {
		RCC->APB1ENR1 &= ~RCC_APB1ENR1_TIM2EN;
	} else {
		RCC->APB1ENR1 &= ~RCC_APB2ENR_TIM1EN;
	}
}
