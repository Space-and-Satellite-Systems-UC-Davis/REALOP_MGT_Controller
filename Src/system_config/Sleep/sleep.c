


#include "sleep.h"


//b0


// bool is_not_init_RTC() { return !(RTC->ISR & RTC_ISR_INITF); }
// bool is_WUTWF_not_ready() { return (RTC->ISR & RTC_ISR_WUTWF) == 0; }
void backup_domain_controlEnable() {
	PWR->CR1 |= PWR_CR1_DBP;
}

void backup_domain_controlDisable() {
	PWR->CR1 &= ~PWR_CR1_DBP;
}

void sleep_init() {
	NVIC_DisableIRQ(SysTick_IRQn); // disable systick interrupts

	__WFE();

	NVIC_EnableIRQ(SysTick_IRQn);
}

void lowPowerSleep_init() {
	NVIC_DisableIRQ(SysTick_IRQn); // disable systick interrupts

	PWR->CR1 |= PWR_CR1_LPR_Msk; //set into low power mode

	__WFE();	

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
	__WFE();
	USART1->CR1 &= ~USART_CR1_UESM;


	NVIC_EnableIRQ(SysTick_IRQn);
}

void rtc_openWritingPrivilege() {
	// Allow Backup Domain Writing Access
	PWR->CR1 |= PWR_CR1_DBP;

	// Enable RTC Write Privilege
	RTC->WPR = RTC_WPR_WRITE_PROTECT_ON_1;
	RTC->WPR = RTC_WPR_WRITE_PROTECT_ON_2;

	// Enter RTC Initialization mode, wait for confirmation of initialization mode
	RTC->ISR |= RTC_ISR_INIT;
	while(is_not_init_RTC()) {}
}

void rtc_closeWritingPrivilege() {
	// Exit Initialization Mode
	RTC->ISR &= ~RTC_ISR_INIT;

	// Enable RTC Write Protection
	RTC->WPR = RTC_WPR_WRITE_PROTECT_OFF;

	// Close Backup Domain Writing Access
	PWR->CR1 &= ~PWR_CR1_DBP;
}

void rtc_config(char clock_source, int forced_config) {
	// do nothing if clock is already configured
	if ((RTC->ISR & RTC_ISR_INITS) && !forced_config) {
		return;
	}

	backup_domain_controlEnable();

	// store the current clock configuration, in case of bad input
	uint32_t temp = RCC->BDCR | RCC_BDCR_RTCSEL;

	// reset the clock
	RCC->BDCR &= ~RCC_BDCR_RTCSEL;

	// Select the RTC clock source
	switch (clock_source) {
		case LSE:
			RCC->BDCR |= RCC_BDCR_RTCSEL_0;
			break;
		case LSI:
			RCC->BDCR |= RCC_BDCR_RTCSEL_1;
			break;
		case HSE:
			RCC->BDCR |= RCC_BDCR_RTCSEL_Msk;
			break;
		default:
			RCC->BDCR |= temp;	// restore the original configuration
			break;
	}

	// Enable the RTC Clock
	RCC->BDCR |= RCC_BDCR_RTCEN;

	backup_domain_controlDisable();

	rtc_openWritingPrivilege();

	// Select the RTC clock source
	switch (clock_source) {
		case LSE:
			RTC->PRER =
				(127 << RTC_PRER_PREDIV_A_Pos)
				| (255 << RTC_PRER_PREDIV_S_Pos);
			break;
		case LSI:
			RTC->PRER =
				(127 << RTC_PRER_PREDIV_A_Pos)
				| (249 << RTC_PRER_PREDIV_S_Pos);
			break;
		case HSE:
			// Depends heavily on the clock source
			break;
		default:
			// Let pre-scalers be whatever they are
			break;
	}

	// Bypass the Shadow registers to read RTC directly
	RTC->CR |= RTC_CR_BYPSHAD;

	rtc_closeWritingPrivilege();

}

bool rtc_wakeUp(uint16_t seconds) {
	// Stop modes not currently implemented
	uint32_t current_source = RCC->BDCR & RCC_BDCR_RTCSEL;
	uint16_t clk_per_sec; //Clock cycles per second
	switch (current_source >> 8) {
		case RTCSEL_LSE:
			clk_per_sec = 2048;
			break;
		case RTCSEL_LSI:
			clk_per_sec = 2000;
			break;
		default:
			return false;
	}
uint32_t MAX_SLEEP = 1000;
	// sleep_cycles = seconds / MAX_SLEEP;
	// sleep_remainder = seconds % MAX_SLEEP;

	// disable write protections
	PWR->CR1 |= PWR_CR1_DBP;

	RTC->CR &= ~(RTC_CR_WUTE); //clear
	//Can't access WUCKSEL/WUT otherwise
	while(is_WUTWF_not_ready()) {}

	//Set auto-reload to number of seconds we wait
	RTC->WUTR &= ~(RTC_WUTR_WUT);
	(seconds > MAX_SLEEP) ?
			(RTC->WUTR |= (MAX_SLEEP * clk_per_sec) << RTC_WUTR_WUT_Pos)
			: (RTC->WUTR |= (seconds * clk_per_sec) << RTC_WUTR_WUT_Pos);

	//Select the clock divisor
	//ck_spre appears to cause ~1.3s delay every interrupt?? Avoid
	RTC->CR &= ~(RTC_CR_WUCKSEL);
	RTC->CR |= (WUCKSEL_RTC_DIV16) << RTC_CR_WUCKSEL_Pos;

	//Configure interrupt
	RTC->ISR &= ~(RTC_ISR_WUTF);
	EXTI->EMR1 |= EXTI_EMR1_EM20;
	// EXTI->RTSR1 |= EXTI_RTSR1_RT20;
	RTC->CR |= RTC_CR_WUTIE;
	NVIC_EnableIRQ(RTC_WKUP_IRQn);

	// RTC->CR |= RTC_CR_WUTE; // Enable wakeup

	rtc_closeWritingPrivilege();
	return true;
}
