


#include "sleep.h"
#include "RTC/rtc.h"
//b0
// a01

void sleep_init() {
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; // Disable the systick
	// SCB->SCR &= ~SCB_SCR_SEVONPEND_Msk;
	SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
	// __DSB();
	__WFI();
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; // Disable the systick
}

void lowPowerSleep_init() {
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; // Disable the systick
	
	PWR->CR1 |= PWR_CR1_LPR_Msk; //set into low power mode
	SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;

	__WFI();	

	PWR->CR1 &= ~PWR_CR1_LPR_Msk; //leave low power mode
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; // Disable the systick
}

void stop0_init() {
	// USART1->CR1; // make usart able to wake up stop mode
	RCC->CR |= RCC_CR_HSIKERON; // keep HSI clock on during sleep mode
	RCC->CFGR &= ~RCC_CFGR_STOPWUCK; // make sure the cpu is set to main clock on wakeup
	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
	PWR->CR1 &= ~PWR_CR1_LPMS; //set the LPMS bit to "000"
	USART1->CR1 |= USART_CR1_UESM;
	__WFI();
	USART1->CR1 &= ~USART_CR1_UESM;


}

