


#include "sleep.h"
#include "globals.h"


void sleep_init() {
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; // Disable the systick
	SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
	__WFI();
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; // Enable the systick
}

// void lowPowerSleep_init() {
// 	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; // Disable the systick


// 	RCC->CFGR &= ~(RCC_CFGR_HPRE);
// 	// Divide SYSCLK by 64. 80MHz -> 5MHz
// 	// Assuming PLL 80MHz SYSCLK
// 	RCC->CFGR |= RCC_CFGR_HPRE_DIV16;
// 	core_MHz = 5;
// 	PWR->CR1 |= PWR_CR1_LPR; //set into low power mode

// 	__WFI();	

// 	PWR->CR1 &= ~(PWR_CR1_LPR);
// 	while((PWR->SR2 & PWR_SR2_REGLPF) != true);
// 	RCC->CFGR &= ~(RCC_CFGR_HPRE); // Reset clock divisor
// 	core_MHz = 80;
// 	SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
	

	
// 	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; // Enable the systick
// }

// void stop0_init() {
// 	// USART1->CR1; // make usart able to wake up stop mode
// 	USART1->CR3 |= USART_CR3_UCESM;
// 	RCC->CR |= RCC_CR_HSIKERON; // keep HSI clock on during sleep mode
// 	RCC->CFGR &= ~RCC_CFGR_STOPWUCK; // make sure the cpu is set to main clock on wakeup
// 	PWR->CR1 &= ~PWR_CR1_LPMS_STOP0; //set the LPMS bit to "000"

// 	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
	
// 	USART1->CR1 |= USART_CR1_UESM;

// 	// __WFI();


// 	USART1->CR1 &= ~USART_CR1_UESM;
// 	USART1->CR3 &= ~USART_CR3_UCESM;
// 	SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;


// }

