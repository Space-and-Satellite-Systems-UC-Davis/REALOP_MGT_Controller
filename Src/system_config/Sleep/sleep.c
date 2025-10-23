


#include "sleep.h"





void sleep_init() {
   NVIC_DisableIRQ(SysTick_IRQn); // disable systick interrupts

   __WFI();

   NVIC_EnableIRQ(SysTick_IRQn);
}

void lowPowerSleep_init() {
   NVIC_DisableIRQ(SysTick_IRQn); // disable systick interrupts

   PWR->CR1 |= PWR_CR1_LPR_Msk; //set into low power mode

   __WFI();

   PWR->CR1 &= ~PWR_CR1_LPR_Msk; //leave low power mode
   NVIC_EnableIRQ(SysTick_IRQn);
}

void stop0_init() {
   // USART1->CR1; // make usart able to wake up stop mode
   RCC->CR |= RCC_CR_HSIKERON; // keep HSI clock on during sleep mode
   RCC->CFGR &= ~RCC_CFGR_STOPWUCK; // make sure the cpu is set to main clock on wakeup
   NVIC_DisableIRQ(SysTick_IRQn);  // disable systick interupts
   
   PWR->CR1 &= 0xFFFFFFF8; //set the LPMS bit to "000"
   USART1->CR1 |= USART_CR1_UESM;
   __WFI();
   USART1->CR1 &= ~USART_CR1_UESM;

   
   NVIC_EnableIRQ(SysTick_IRQn);
}