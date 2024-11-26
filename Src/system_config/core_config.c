/*
 * core_config.c
 *
 *  - August 18, 2023
 *  	Author	: Darsh
 *  	Log		: Copied from IntelliSat, modified for Radio Controller
 */

#include "core_config.h"

// Global variable
int core_MHz;

void init_coreClocks() {
	FLASH->ACR |=
		  FLASH_ACR_DCEN			// Flash data cache enable
		| FLASH_ACR_ICEN			// Flash instruction cache enable
		| FLASH_ACR_PRFTEN			// Flash prefetch enable
		| FLASH_ACR_LATENCY_4WS;	// 4 HCLCK periods of latency in Flash access time

	RCC->APB1ENR1 |= RCC_APB1ENR1_PWREN; 	// enable Power Control
	RCC->APB2ENR  |= RCC_APB2ENR_SYSCFGEN;	//

	// enable internal oscillators
	RCC->CR |= RCC_CR_HSION; 					// enable HSI
	RCC->CSR |= RCC_CSR_LSION;					// Turn on the LSI Oscillator
	while (!(RCC->CSR & RCC_CSR_LSIRDY));		// wait for the LSI Oscillator to stabilize

	// configure Phased Lock Loop
	RCC->PLLCFGR =
		  0 << RCC_PLLCFGR_PLLR_Pos 	// PLLR = 2
		| RCC_PLLCFGR_PLLREN 			// PLLCLK output enabled
		| 0 << RCC_PLLCFGR_PLLQ_Pos 	//
		| RCC_PLLCFGR_PLLQEN			// PLLQEN (PLL48M1CLK) output enabled
		| 0 << RCC_PLLCFGR_PLLP_Pos 	// PLLP default
		| 0 << RCC_PLLCFGR_PLLPEN_Pos 	// PLLSAI3 not enabled
		| (10 << RCC_PLLCFGR_PLLN_Pos) 	// PLLN = 10
		| 0 << RCC_PLLCFGR_PLLM_Pos 	// PLLM = 1
		| RCC_PLLCFGR_PLLSRC_HSI; 		// HSI16 as input clock to PLLs
	RCC->PLLSAI1CFGR =
		  RCC_PLLSAI1CFGR_PLLSAI1R_0		 	// PLLADC1CLK division factor set to 4
		| RCC_PLLSAI1CFGR_PLLSAI1REN 			// PLLADC1CLK output enable
		| RCC_PLLSAI1CFGR_PLLSAI1Q_0 			// PLL48M2CLK division factor set to 4
		| RCC_PLLSAI1CFGR_PLLSAI1QEN 			// PLL48M2CLK output enable
		| 0 << RCC_PLLSAI1CFGR_PLLSAI1P_Pos		// PLLSAI1CLKC division factor set to 7
		| 0 << RCC_PLLSAI1CFGR_PLLSAI1PEN_Pos 	// PLLSAI1CLK output disabled
		| 12 << RCC_PLLSAI1CFGR_PLLSAI1N_Pos; 	// VCO multiplication factor set to 12
	// enable PLL and PLLSAI1
	RCC->CR |= RCC_CR_PLLON | RCC_CR_PLLSAI1ON;
	while (!(RCC->CR & RCC_CR_PLLRDY));
	while (!(RCC->CR & RCC_CR_PLLSAI1RDY));
	RCC->CFGR = RCC_CFGR_SW;	// system clock to PLL

	// configure UART to use HSI16
	RCC->CCIPR |=
		  (2U << RCC_CCIPR_LPUART1SEL_Pos)
		| (2U << RCC_CCIPR_USART3SEL_Pos)
		| (2U << RCC_CCIPR_USART2SEL_Pos)
		| (2U << RCC_CCIPR_USART1SEL_Pos);

	// configure APB clocks to be System_Clock / 16
	RCC->CFGR |=
		  (7U << RCC_CFGR_PPRE1_Pos)	// APB1
		| (7U << RCC_CFGR_PPRE2_Pos);	// APB2

	core_MHz = 80;


	// clock all GPIO ports
	// - GPIO ports being unclocked is a reason drivers don't work, and is hard to debug
	RCC->AHB2ENR |=
		  RCC_AHB2ENR_GPIOAEN
		| RCC_AHB2ENR_GPIOBEN
		| RCC_AHB2ENR_GPIOCEN
		| RCC_AHB2ENR_GPIODEN
		| RCC_AHB2ENR_GPIOEEN
		| RCC_AHB2ENR_GPIOHEN;
	// wait until each GPIO is clocked and ready
	while (GPIOA->OTYPER == 0xFFFFFFFF);
	while (GPIOB->OTYPER == 0xFFFFFFFF);
	while (GPIOC->OTYPER == 0xFFFFFFFF);
	while (GPIOD->OTYPER == 0xFFFFFFFF);
	while (GPIOE->OTYPER == 0xFFFFFFFF);
	while (GPIOH->OTYPER == 0xFFFFFFFF);
}


// ----------------------------------------------------------------------------

void nop(long long nop_loops) {
	for (long long i = 0; i < nop_loops; i++) {__NOP();}
}
